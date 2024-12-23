#ifndef nethttp_http_client
#define nethttp_http_client

#include <netsock/tcp_stream.hpp>

#include "http_response.hpp"
#include "http_request.hpp"
#include "url.hpp"
#include "netsock/tcp_client.hpp"
#include "netsock/dns.hpp"

namespace nethttp {
    class http_client {
    public:
        http_client()= default;

        http_response send_request(const http_method method, const url &url, const http_body &body = empty_body(), const http_headers &headers = {}) {
            http_request request(method);
            request.set_headers(headers);
            request.set_body(body);
            for (const auto &[name, values] : _default_headers) {
                if (!request.has_header(name))
                    request.add_header(name, values);
            }
            request.set_path(url.full_path());
            if (!request.has_header("Host"))
                request.add_header("Host", url.host());
            netsock::tcp_client client;
            netsock::tcp_stream stream(client);
            const auto addresses = netsock::dns::resolve(url.host());
            if (addresses.empty())
                throw std::invalid_argument("url resolution failed");
            for (const auto &address : addresses) {
                try {
                    client.connect(address & (url.port() == 0 ? 80 : url.port()));
                    if (client.connected())
                        break;
                } catch (const netsock::socket_error &e) {

                }
            }
            if (!client.connected()) {
                throw std::invalid_argument("connection failed");
            }
            http_response_message response;
            to_stream(request.message(), stream).flush();
            from_stream(response, stream);
            return response;
        }

        http_response get_request(const url &url, const http_headers &headers = {}) {
            return send_request(GET, url, empty_body(), headers);
        }

        http_response post_request(const url &url, const http_body &body, const http_headers &headers = {}) {
            return send_request(POST, url, body, headers);
        }

        void add_default_header(std::string name, http_header_values values) {
            _default_headers.add(std::move(name), std::move(values));
        }

        void set_default_headers(const http_headers &default_headers) {
            _default_headers = default_headers;
        }

        [[nodiscard]] const http_headers &default_headers() const {
            return _default_headers;
        }
    private:
        http_headers _default_headers;
    };
}

#endif //nethttp_http_client
