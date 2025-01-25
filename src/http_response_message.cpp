#include "nethttp/http_response_message.hpp"

namespace nethttp {
    http_response_message::http_response_message(const std::int16_t status, std::string message): _status(status), _message(std::move(message)) {

    }

    void http_response_message::set_status(const std::int16_t status) {
        _status = status;
    }

    std::int16_t http_response_message::status() const {
        return _status;
    }

    void http_response_message::set_message(std::string message) {
        _message = std::move(message);
    }

    const std::string & http_response_message::message() const {
        return _message;
    }

    void http_response_message::set_version(const std::uint16_t high, const std::uint16_t low) {
        _version = make_version(high, low);
    }

    nethttp::version http_response_message::version() const {
        return _version;
    }

    void http_response_message::set_headers(http_headers headers) {
        _headers = std::move(headers);
    }

    http_headers & http_response_message::headers() {
        return _headers;
    }

    const http_headers & http_response_message::headers() const {
        return _headers;
    }

    void http_response_message::set_body(std::string body) {
        _body = std::move(body);
    }

    std::string & http_response_message::raw_body() {
        return _body;
    }

    const std::string & http_response_message::raw_body() const {
        return _body;
    }

    std::string http_response_message::body() const {
        return parse_body(_body, _headers);
    }

    std::ostream & to_stream(const http_response_message &message, std::ostream &os) {
        return to_stream(message.headers(), to_stream(message.version(), os) << ' ' << message.status() << ' ' << message.message() << "\r\n") << message.body();
    }

    std::string to_string(const http_response_message &message) {
        std::stringstream ss;
        to_stream(message, ss);
        return ss.str();
    }

    std::istream & from_stream(http_response_message &response, std::istream &is) {
        version version;
        if (!from_stream(version, is))
            return is;
        response.set_version(version.first, version.second);
        std::int16_t status;
        if (!(is >> status))
            return is;
        response.set_status(status);
        std::string message;
        if (std::getline(is, message, '\r').fail())
            return is;
        if (is.get() != '\n') {
            is.setstate(std::ios::failbit);
            return is;
        }
        response.set_message(message);
        auto &headers = response.headers();
        if (!from_stream(headers, is))
            return is;
        std::string body;
        if (!read_body(body, headers, is))
            return is;
        response.set_body(body);
        return is;
    }

    bool from_string(http_response_message &response, const std::string &string) {
        std::stringstream ss(string);
        from_stream(response, ss);
        return ss.good();
    }
}
