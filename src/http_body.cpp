#include "nethttp/http_body.hpp"

namespace nethttp {
    http_body::http_body()= default;
    http_body::~http_body()= default;

    empty_body::empty_body()= default;

    void empty_body::add_to_message(std::string &body, http_headers &headers) const {
        body.clear();
        headers.remove("Content-Type");
        headers.remove("Content-Length");
    }

    string_body::string_body(std::string body, std::string type, std::string charset): _body(std::move(body)), _type(std::move(type)), _charset(std::move(charset)) {

    }

    const std::string & string_body::body() const {
        return _body;
    }

    void string_body::set_type(std::string type) {
        _type = std::move(type);
    }

    const std::string & string_body::type() const {
        return _type;
    }

    void string_body::set_charset(std::string charset) {
        _charset = std::move(charset);
    }

    const std::string & string_body::charset() const {
        return _charset;
    }

    void string_body::add_to_message(std::string &body, http_headers &headers) const {
        body = _body;
        headers.add_or_set("Content-Length", std::to_string(_body.size()));
        http_header_values content_type;
        if (!_charset.empty())
            content_type = _type + ';' + "charset=" + _charset;
        else
            content_type = _type;
        headers.add_or_set("Content-Type", std::move(content_type));
    }

    html_body::html_body(std::string body, std::string charset): string_body(std::move(body), "text/html", std::move(charset)) {

    }

    image_body::image_body(std::string body, std::string image_extension): string_body(std::move(body), "image/"+std::move(image_extension)) {

    }
}
