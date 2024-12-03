#ifndef nethttp_http_request
#define nethttp_http_request

#include "http_body.hpp"
#include "http_request_message.hpp"

namespace nethttp {
    class http_request {
    public:
        http_request(http_request_message message): _message(std::move(message)) {

        }

        http_request()= default;

        explicit http_request(const http_method method): _message(method) {

        }

        void set_method(const http_method method) {
            _message.set_method(method);
        }

        [[nodiscard]] http_method method() const {
            return _message.method();
        }

        void set_path(const std::string &path) {
            _message.set_path(path);
        }

        [[nodiscard]] const std::string &path() const {
            return _message.path();
        }

        void set_version(const std::uint16_t major, const std::uint16_t minor) {
            _message.set_version(major, minor);
        }

        [[nodiscard]] nethttp::version version() const {
            return _message.version();
        }

        void set_headers(const http_headers &headers) {
            _message.set_headers(headers);
        }

        void add_header(const std::string &name, const http_header_values &value) {
            if (_message.headers().has(name)) {
                _message.headers()[name] = value;
                return;
            }
            _message.headers().add(name, value);
        }

        [[nodiscard]] bool has_header(const std::string &name) const {
            return _message.headers().has(name);
        }

        [[nodiscard]] http_header_values &get_header(const std::string &name) {
            return _message.headers().get(name);
        }

        [[nodiscard]] const http_header_values &get_header(const std::string &name) const {
            return _message.headers().get(name);
        }

        [[nodiscard]] http_headers &headers() {
            return _message.headers();
        }

        [[nodiscard]] const http_headers &headers() const {
            return _message.headers();
        }

        void set_body(const std::string &body) {
            _message.set_body(body);
        }

        void set_body(const http_body &body) {
            body.add_to_message(_message.raw_body(), _message.headers());
        }

        [[nodiscard]] std::string body() const {
            return _message.body();
        }

        [[nodiscard]] const std::string &raw_body() const {
            return _message.raw_body();
        }

        [[nodiscard]] http_request_message &message() {
            return _message;
        }

        [[nodiscard]] const http_request_message &message() const {
            return _message;
        }

        [[nodiscard]] const std::string &wildcard(const std::size_t i = 0) const {
            return _wildcards[i];
        }
    private:
        friend class http_server;
        http_request_message _message;
        std::vector<std::string> _wildcards;
    };
}

#endif //nethttp_http_request
