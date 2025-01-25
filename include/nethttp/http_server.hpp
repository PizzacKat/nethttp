#ifndef nethttp_http_server
#define nethttp_http_server

#include <functional>

#include "netsock/tcp_listener.hpp"
#include "netsock/tcp_stream.hpp"
#include "nethttp/http_response.hpp"
#include "nethttp/http_request.hpp"
#include <utility>

namespace nethttp {
    class http_server {
    public:
        http_server();
        explicit http_server(const netsock::ip_endpoint &endpoint);
        http_server(const netsock::ip_address &address, std::uint16_t port);

        void listen(const netsock::ip_endpoint &endpoint);
        void listen(const netsock::ip_address &address, std::uint16_t port);

        void add_endpoint(int methods, const std::string &path, const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback);

        void start();
        void stop();

    private:
        static bool endpoint_match(const std::string &endpoint_path, const std::string &path, std::vector<std::string> &wildcards);

        struct endpoint {
            int methods;
            std::string path;
            std::function<void(http_server &server, const http_request &request, http_response &response)> callback;

            endpoint(const int methods, std::string path, const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback): methods(methods), path(std::move(path)), callback(callback) {

            }
        };

        netsock::tcp_listener _listener;
        std::vector<endpoint> _endpoints;
    };
}

#endif //nethttp_http_server
