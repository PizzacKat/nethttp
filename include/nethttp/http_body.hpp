#ifndef nethttp_http_body
#define nethttp_http_body

#include <fstream>

#include "http_headers.hpp"

namespace nethttp {
    class http_body {
    public:
        http_body()= default;

        virtual void add_to_message(std::string &body, http_headers &headers) const= 0;

        virtual ~http_body()= default;
    };

    class empty_body : public http_body {
    public:
        empty_body()= default;

        void add_to_message(std::string &body, http_headers &headers) const override {
            body.clear();
            headers.remove("Content-Type");
            headers.remove("Content-Length");
        }
    };

    class string_body : public http_body {
    public:
        explicit string_body(std::string body, std::string type = "text/plain", std::string charset = ""): _body(std::move(body)), _type(std::move(type)), _charset(std::move(charset)) {

        }

        [[nodiscard]] const std::string &body() const {
            return _body;
        }

        void set_type(std::string type) {
            _type = std::move(type);
        }

        [[nodiscard]] const std::string &type() const {
            return _type;
        }

        void set_charset(std::string charset) {
            _charset = std::move(charset);
        }

        [[nodiscard]] const std::string &charset() const {
            return _charset;
        }

        void add_to_message(std::string &body, http_headers &headers) const override {
            body = _body;
            headers.add_or_set("Content-Length", std::to_string(_body.size()));
            http_header_values content_type;
            if (!_charset.empty())
                content_type = _type + ';' + "charset=" + _charset;
            else
                content_type = _type;
            headers.add_or_set("Content-Type", std::move(content_type));
        }

    private:
        std::string _body;
        std::string _type;
        std::string _charset;
    };

    class html_body : public string_body {
    public:
        explicit html_body(std::string body, std::string charset = ""): string_body(std::move(body), "text/html", std::move(charset)) {

        }
    };

    class image_body : public string_body {
    public:
        explicit image_body(std::string body, std::string image_extension): string_body(std::move(body), "image/"+std::move(image_extension)) {

        }
    };

    template <typename T, typename... Args>
    T from_file(const std::string &filename, Args &&... args) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Could not open file " + filename);
        std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        file.close();
        return T(content, std::forward<Args>(args)...);
    }
}

#endif //nethttp_http_body
