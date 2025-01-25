#include "nethttp/http_request.hpp"

namespace nethttp {
    http_request::http_request(http_request_message message): _message(std::move(message)) {

    }

    http_request::http_request()= default;

    http_request::http_request(const http_method method): _message(method) {

    }

    void http_request::set_method(const http_method method) {
        _message.set_method(method);
    }

    http_method http_request::method() const {
        return _message.method();
    }

    void http_request::set_path(const std::string &path) {
        _message.set_path(path);
    }

    const std::string & http_request::path() const {
        return _message.path();
    }

    void http_request::set_version(const std::uint16_t major, const std::uint16_t minor) {
        _message.set_version(major, minor);
    }

    nethttp::version http_request::version() const {
        return _message.version();
    }

    void http_request::set_headers(const http_headers &headers) {
        _message.set_headers(headers);
    }

    void http_request::add_header(const std::string &name, const http_header_values &value) {
        if (_message.headers().has(name)) {
            _message.headers()[name] = value;
            return;
        }
        _message.headers().add(name, value);
    }

    bool http_request::has_header(const std::string &name) const {
        return _message.headers().has(name);
    }

    http_header_values & http_request::get_header(const std::string &name) {
        return _message.headers().get(name);
    }

    const http_header_values & http_request::get_header(const std::string &name) const {
        return _message.headers().get(name);
    }

    http_headers & http_request::headers() {
        return _message.headers();
    }

    const http_headers & http_request::headers() const {
        return _message.headers();
    }

    void http_request::set_body(const std::string &body) {
        _message.set_body(body);
    }

    void http_request::set_body(const http_body &body) {
        body.add_to_message(_message.raw_body(), _message.headers());
    }

    std::string http_request::body() const {
        return _message.body();
    }

    const std::string & http_request::raw_body() const {
        return _message.raw_body();
    }

    http_request_message & http_request::message() {
        return _message;
    }

    const http_request_message & http_request::message() const {
        return _message;
    }

    const std::string & http_request::wildcard(const std::size_t i) const {
        return _wildcards[i];
    }
}
