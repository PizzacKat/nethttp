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
        typedef std::function<void(http_server &server, const http_request &request, http_response &response)> endpoint_callback;
        typedef std::function<void(http_server &server, const std::exception &exception, http_response &response)> error_callback;

        http_server();
        explicit http_server(const netsock::ip_endpoint &endpoint);
        http_server(const netsock::ip_address &address, std::uint16_t port);

        void listen(const netsock::ip_endpoint &endpoint);
        void listen(const netsock::ip_address &address, std::uint16_t port);

        [[nodiscard]] bool listening() const;

        void add_endpoint(int methods, const std::string &path, const endpoint_callback &callback);

        void set_default_callback(const endpoint_callback &default_callback);
        void set_exception_callback(const error_callback &exception_callback);

        void start();
        void stop();

    private:
        static bool endpoint_match(const std::string &endpoint_path, const std::string &path, std::vector<std::string> &wildcards);

        struct endpoint {
            int methods;
            std::string path;
            endpoint_callback callback;

            endpoint(const int methods, std::string path, const std::function<void(http_server &server, const http_request &request, http_response &response)> &callback): methods(methods), path(std::move(path)), callback(callback) {

            }
        };

        static void default_exception_callback(http_server &, const std::exception &, http_response &response);
        static void default_callback(http_server &, const http_request &, http_response &response);

        netsock::tcp_listener _listener;
        std::vector<endpoint> _endpoints;
        error_callback _on_exception_callback = default_exception_callback;
        endpoint_callback _default_callback = default_callback;
    };
}

#endif //nethttp_http_server
