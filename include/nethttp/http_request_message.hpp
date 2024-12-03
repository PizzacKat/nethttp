#ifndef nethttp_http_request_message
#define nethttp_http_request_message
#include "body.hpp"
#include "http_headers.hpp"
#include "enums.hpp"

namespace nethttp {
    class http_request_message {
    public:
        explicit http_request_message(const http_method method = GET): _method(method) {

        }

        void set_method(const http_method method) {
            _method = method;
        }

        [[nodiscard]] http_method method() const {
            return _method;
        }

        void set_path(const std::string &path) {
            _path = path;
        }

        [[nodiscard]] const std::string &path() const {
            return _path;
        }

        void set_version(const std::uint16_t high, const std::uint16_t low) {
            _version = make_version(high, low);
        }

        [[nodiscard]] nethttp::version version() const {
            return _version;
        }

        void set_headers(const http_headers &headers) {
            _headers = headers;
        }

        [[nodiscard]] http_headers &headers() {
            return _headers;
        }

        [[nodiscard]] const http_headers &headers() const {
            return _headers;
        }

        void set_body(const std::string &body) {
            _body = body;
        }

        [[nodiscard]] std::string body() const {
            return parse_body(_body, _headers);
        }

        [[nodiscard]] std::string &raw_body() {
            return _body;
        }

        [[nodiscard]] const std::string &raw_body() const {
            return _body;
        }
    private:
        http_method _method;
        std::string _path{"/"};
        nethttp::version _version{1, 1};
        http_headers _headers;
        std::string _body;
    };

    inline std::ostream &to_stream(const http_request_message &message, std::ostream &os) {
        return to_stream(message.headers(), to_stream(message.version(), os << to_string(message.method()) << ' ' << message.path() << ' ') << "\r\n") << message.body();
    }

    inline std::string to_string(const http_request_message &message) {
        std::stringstream ss;
        to_stream(message, ss);
        return ss.str();
    }

    inline std::istream &from_stream(http_request_message &request, std::istream &is) {
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

    inline bool from_string(http_request_message &request, const std::string &string) {
        std::istringstream ss(string);
        from_stream(request, ss);
        return ss.good();
    }
}

#endif //nethttp_http_request_message
