#include "nethttp/http_server.hpp"

#include <thread>

namespace nethttp {
    http_server::http_server() = default;

    http_server::http_server(const netsock::ip_endpoint &endpoint) {
        listen(endpoint);
    }

    http_server::http_server(const netsock::ip_address &address, const std::uint16_t port) {
        listen(address, port);
    }

    void http_server::listen(const netsock::ip_endpoint &endpoint) {
        _listener.bind(endpoint);
    }

    void http_server::listen(const netsock::ip_address &address, const std::uint16_t port) {
        _listener.bind(address, port);
    }

    void http_server::add_endpoint(const int methods, const std::string &path,
        const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback)
    {
        _endpoints.emplace_back(methods, path, callback);
    }

    void http_server::start() {
        _listener.listen(1024);
        while (_listener.listening()) {
            auto client = _listener.accept_client();
            std::thread([this](netsock::tcp_client client){
                netsock::tcp_stream stream(client);
                http_request request;
                if (!from_stream(request.message(), stream)) {
                    http_response response(400, "Bad Request");
                    stream.clear();
                    to_stream(response.message(), stream).flush();
                    client.close();
                    return;
                }
                http_response response(404, "Not Found");
                std::vector<std::pair<std::function<void(http_server &server, const http_request &request, http_response &response)>, std::vector<std::string>>> endpoints;
                for (const auto &[methods, path, callback] : _endpoints)
                    if (methods & request.method() && endpoint_match(path, request.path(), request._wildcards)) {
                        endpoints.emplace_back(callback, request._wildcards);
                    }
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
                    callback(*this, request, response);
                }
                to_stream(response.message(), stream).flush();
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
}
