#ifndef nethttp_http_response
#define nethttp_http_response

#include "http_body.hpp"
#include "http_response_message.hpp"

namespace nethttp {
    class http_response {
    public:
        http_response(http_response_message message): _message(std::move(message)) {

        }

        http_response()= default;

        explicit http_response(const std::int16_t status, std::string message = "OK"): _message(status, std::move(message)) {

        }

        void set_version(const std::uint16_t major, const std::uint16_t minor) {
            _message.set_version(major, minor);
        }

        [[nodiscard]] nethttp::version version() const {
            return _message.version();
        }

        void set_status(const std::int16_t status) {
            _message.set_status(status);
        }

        [[nodiscard]] std::int16_t status() const {
            return _message.status();
        }

        void set_status_message(const std::string &message) {
            _message.set_message(message);
        }

        [[nodiscard]] const std::string &status_message() const {
            return _message.message();
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

        [[nodiscard]] http_response_message &message() {
            return _message;
        }

        [[nodiscard]] const http_response_message &message() const {
            return _message;
        }
    private:
        http_response_message _message;
    };
}

#endif //nethttp_http_response
