#include "nethttp/http_server.hpp"

#include <thread>
#include <netsock/poll_list.hpp>

namespace nethttp {
    http_server::http_server() {
        _listener.reuse_address(true);
    }

    http_server::http_server(const netsock::ip_endpoint &endpoint): http_server() {
        listen(endpoint);
    }

    http_server::http_server(const netsock::ip_address &address, const std::uint16_t port): http_server() {
        listen(address, port);
    }

    void http_server::listen(const netsock::ip_endpoint &endpoint) {
        _listener.bind(endpoint);
    }

    void http_server::listen(const netsock::ip_address &address, const std::uint16_t port) {
        _listener.bind(address, port);
    }

    bool http_server::listening() const {
        return _listener.listening();
    }

    void http_server::add_endpoint(const int methods, const std::string &path, const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback) {
        _endpoints.emplace_back(methods, path, callback);
    }

    void http_server::set_exception_callback(const error_callback &exception_callback) {
        _on_exception_callback = exception_callback;
    }

    void http_server::set_default_callback(const endpoint_callback &default_callback) {
        _default_callback = default_callback;
    }

    static constexpr std::chrono::milliseconds default_keep_alive_timeout{10};

    struct keep_alive {
        bool active = true;
        std::chrono::milliseconds timeout = default_keep_alive_timeout;
        std::size_t requests = -1;
    };

    inline std::pair<std::string, std::string> key_value(const std::string &str) {
        const auto equals = str.find('=');
        return {str.substr(0, equals), str.substr(equals + 1)};
    }

    inline void handle_keep_alive(const http_request &request, keep_alive &keep_alive) {
        if (request.has_header("Connection")) {
            const auto &connection = request.get_header("Connection");
            if (connection.contains("close"))
                keep_alive.active = false;
            if (connection.contains("keep-alive"))
                keep_alive.active = true;
        }

        if (keep_alive.active && request.has_header("Keep-Alive")) {
            for (const auto &arg : request.get_header("Keep-Alive")) {
                const auto [key, value] = key_value(arg);
                if (key == "timeout")
                    keep_alive.timeout = std::chrono::milliseconds(std::stoul(value) * 1000);
                if (key == "max")
                    keep_alive.requests = std::stoul(value);
            }
        }
    }

    void http_server::start() {
        _listener.listen(1024);
        while (_listener.listening()) {
            auto client = _listener.accept_client();
            std::thread([this](netsock::tcp_client client){
                netsock::tcp_stream stream(client);
                http_request request;
                keep_alive keep_alive;
                do {
                    if (!client.poll(netsock::pollin, keep_alive.timeout))
                        break;
                    if (from_stream(request.message(), stream).bad()) {
                        http_response response(400, "Bad Request");
                        stream.clear();
                        to_stream(response.message(), stream).flush();
                        client.close();
                        return;
                    }
                    handle_keep_alive(request, keep_alive);
                    http_response response(404, "Not Found");
                    std::vector<std::pair<std::function<void(http_server &server, const http_request &request, http_response &response)>, std::vector<std::string>>> endpoints;
                    for (const auto &[methods, path, callback] : _endpoints)
                        if (methods & request.method() && endpoint_match(path, request.path(), request._wildcards)) {
                            endpoints.emplace_back(callback, request._wildcards);
                        }
                    bool found = false;
                    if (!endpoints.empty()) {
                        std::function<void(http_server &server, const http_request &request, http_response &response)> callback;
                        std::size_t less = 999999;
                        for (const auto &endpoint : endpoints) {
                            if (endpoint.second.size() < less) {
                                less = endpoint.second.size();
                                callback = endpoint.first;
                            }
                        }
                        response = http_response(200, "OK");
                        try {
                            callback(*this, request, response);
                        } catch (const std::exception &exception) {
                            _on_exception_callback(*this, exception, response);
                        }
                        found = true;
                    }
                    if (!found)
                        _default_callback(*this, request, response);
                    to_stream(response.message(), stream).flush();
                    keep_alive.requests--;
                } while (keep_alive.active && keep_alive.requests > 0);
                client.close();
            }, std::move(client)).detach();
        }
    }

    void http_server::stop() {
        _listener.close();
    }

    bool http_server::endpoint_match(const std::string &endpoint_path, const std::string &path,
        std::vector<std::string> &wildcards) {
        const char *epath = endpoint_path.c_str();
        const char *cpath = path.c_str();
        wildcards.clear();
        while (*epath != 0) {
            const char *wcbeg = cpath;
            if (*epath == '*') {
                while (*cpath != '/' && *cpath != 0)
                    cpath++;
                wildcards.emplace_back(wcbeg, cpath);
            }
            else {
                if (*epath != *cpath) {
                    wildcards.clear();
                    return false;
                }
            }
            if (*epath == '*')
                cpath--;
            epath++;
            cpath++;
        }
        if (*cpath != 0)
            return false;
        return true;
    }

    void http_server::default_exception_callback(http_server &, const std::exception &, http_response &response) {
        response = http_response(500, "Internal Server Error");
    }

    void http_server::default_callback(http_server &, const http_request &, http_response &response) {
        response = http_response(404, "Not Found");
    }

}
