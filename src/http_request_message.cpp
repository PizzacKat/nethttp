#include "nethttp/http_request_message.hpp"
#include <sstream>
#include "nethttp/body.hpp"

namespace nethttp {
    http_request_message::http_request_message(const http_method method): _method(method) {

    }

    void http_request_message::set_method(const http_method method) {
        _method = method;
    }

    http_method http_request_message::method() const {
        return _method;
    }

    void http_request_message::set_path(const std::string &path) {
        _path = path;
    }

    const std::string &http_request_message::path() const {
        return _path;
    }

    void http_request_message::set_version(const std::uint16_t high, const std::uint16_t low) {
        _version = make_version(high, low);
    }

    nethttp::version http_request_message::version() const {
        return _version;
    }

    void http_request_message::set_headers(const http_headers &headers) {
        _headers = headers;
    }

    http_headers &http_request_message::headers() {
        return _headers;
    }

    const http_headers & http_request_message::headers() const {
        return _headers;
    }

    void http_request_message::set_body(const std::string &body) {
        _body = body;
    }

    std::string http_request_message::body() const {
        return parse_body(_body, _headers);
    }

    std::string &http_request_message::raw_body() {
        return _body;
    }

    const std::string &http_request_message::raw_body() const {
        return _body;
    }

    std::ostream &to_stream(const http_request_message &message, std::ostream &os) {
        return to_stream(message.headers(), to_stream(message.version(), os << to_string(message.method()) << ' ' << message.path() << ' ') << "\r\n") << message.body();
    }

    std::string to_string(const http_request_message &message) {
        std::stringstream ss;
        to_stream(message, ss);
        return ss.str();
    }

    std::istream &from_stream(http_request_message &request, std::istream &is) {
        std::string method_string;
        http_method method;
        is >> method_string;
        if (is.get() != ' ' || !from_string(method, method_string)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        request.set_method(method);
        std::string path;
        is >> path;
        if (is.get() != ' ') {
            is.setstate(std::ios::failbit);
            return is;
        }
        request.set_path(path);
        nethttp::version version;
        if (!from_stream(version, is))
            return is;
        if (is.get() != '\r' || is.get() != '\n') {
            is.setstate(std::ios::failbit);
            return is;
        }
        request.set_version(version.first, version.second);
        auto &headers = request.headers();
        if (!from_stream(headers, is))
            return is;
        std::string body;
        if (!read_body(body, headers, is))
            return is;
        request.set_body(body);
        return is;
    }

    bool from_string(http_request_message &request, const std::string &string) {
        std::istringstream ss(string);
        from_stream(request, ss);
        return ss.good();
    }
}
