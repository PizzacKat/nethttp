#ifndef nethttp_http_server
#define nethttp_http_server

#include <functional>
#include <map>

#include "netsock/tcp_listener.hpp"
#include "netsock/tcp_stream.hpp"
#include "nethttp/http_response.hpp"
#include "nethttp/http_request.hpp"
#include <thread>

namespace nethttp {
    class http_server {
    public:
        http_server()= default;
        explicit http_server(const netsock::ip_endpoint &endpoint) {
            listen(endpoint);
        }

        http_server(const netsock::ip_address &address, const std::uint16_t port) {
            listen(address, port);
        }

        void listen(const netsock::ip_endpoint &endpoint) {
            _listener.bind(endpoint);
        }

        void listen(const netsock::ip_address &address, const std::uint16_t port) {
            _listener.bind(address, port);
        }

        void add_endpoint(const http_method method, const std::string &path, const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback) {
            _endpoints.emplace_back(method, path, callback);
        }

        void start() {
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

        void stop() {
            _listener.close();
        }
    private:
        bool endpoint_match(const std::string &endpoint_path, const std::string &path, std::vector<std::string> &wildcards) {
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

        struct endpoint {
            int methods;
            std::string path;
            std::function<void(http_server &server, const http_request &request, http_response &response)> callback;
        };

        netsock::tcp_listener _listener;
        std::vector<endpoint> _endpoints;
    };
}

#endif //nethttp_http_server
