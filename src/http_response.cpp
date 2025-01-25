#include "nethttp/http_response.hpp"

namespace nethttp {
    http_response::http_response(http_response_message message): _message(std::move(message)) {

    }

    http_response::http_response() = default;

    http_response::http_response(const std::int16_t status, std::string message): _message(status, std::move(message)) {

    }

    void http_response::set_version(const std::uint16_t major, const std::uint16_t minor) {
        _message.set_version(major, minor);
    }

    nethttp::version http_response::version() const {
        return _message.version();
    }

    void http_response::set_status(const std::int16_t status) {
        _message.set_status(status);
    }

    std::int16_t http_response::status() const {
        return _message.status();
    }

    void http_response::set_status_message(const std::string &message) {
        _message.set_message(message);
    }

    const std::string & http_response::status_message() const {
        return _message.message();
    }

    void http_response::set_headers(const http_headers &headers) {
        _message.set_headers(headers);
    }

    void http_response::add_header(const std::string &name, const http_header_values &value) {
        if (_message.headers().has(name)) {
            _message.headers()[name] = value;
            return;
        }
        _message.headers().add(name, value);
    }

    bool http_response::has_header(const std::string &name) const {
        return _message.headers().has(name);
    }

    http_header_values & http_response::get_header(const std::string &name) {
        return _message.headers().get(name);
    }

    const http_header_values & http_response::get_header(const std::string &name) const {
        return _message.headers().get(name);
    }

    http_headers & http_response::headers() {
        return _message.headers();
    }

    const http_headers & http_response::headers() const {
        return _message.headers();
    }

    void http_response::set_body(const std::string &body) {
        _message.set_body(body);
    }

    void http_response::set_body(const http_body &body) {
        body.add_to_message(_message.raw_body(), _message.headers());
    }

    std::string http_response::body() const {
        return _message.body();
    }

    const std::string & http_response::raw_body() const {
        return _message.raw_body();
    }

    http_response_message &http_response::message() {
        return _message;
    }

    const http_response_message &http_response::message() const {
        return _message;
    }
}
