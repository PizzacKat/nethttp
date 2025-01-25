#ifndef nethttp_http_client
#define nethttp_http_client

#include "http_response.hpp"
#include "http_request.hpp"
#include "url.hpp"
#include "netsock/dns.hpp"

namespace nethttp {
    class http_request_error : public std::runtime_error {
    public:
        explicit http_request_error(const std::string &str): std::runtime_error(str) {

        }

        explicit http_request_error(const char *str): std::runtime_error(str) {

        }
    };

    class http_client {
    public:
        http_client();

        http_response send_request(http_method method, const url &url, const http_body &body = empty_body(), const http_headers &headers = {});
        http_response get_request(const url &url, const http_headers &headers = {});
        http_response post_request(const url &url, const http_body &body, const http_headers &headers = {});

        void add_default_header(std::string name, http_header_values values);
        void set_default_headers(const http_headers &default_headers);
        [[nodiscard]] const http_headers &default_headers() const;

    private:
        http_headers _default_headers;
    };
}

#endif //nethttp_http_client
