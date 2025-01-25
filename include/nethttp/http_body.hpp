#ifndef nethttp_http_body
#define nethttp_http_body

#include <fstream>

#include "http_headers.hpp"

namespace nethttp {
    class http_body {
    public:
        http_body();

        virtual void add_to_message(std::string &body, http_headers &headers) const= 0;

        virtual ~http_body();
    };

    class empty_body : public http_body {
    public:
        empty_body();

        void add_to_message(std::string &body, http_headers &headers) const override;
    };

    class string_body : public http_body {
    public:
        explicit string_body(std::string body, std::string type = "text/plain", std::string charset = "");

        [[nodiscard]] const std::string &body() const;

        void set_type(std::string type);

        [[nodiscard]] const std::string &type() const;

        void set_charset(std::string charset);

        [[nodiscard]] const std::string &charset() const;

        void add_to_message(std::string &body, http_headers &headers) const override;

    private:
        std::string _body;
        std::string _type;
        std::string _charset;
    };

    class html_body : public string_body {
    public:
        explicit html_body(std::string body, std::string charset = "");
    };

    class image_body : public string_body {
    public:
        explicit image_body(std::string body, std::string image_extension);
    };

    template <typename T, typename... Args>
    T from_file(const std::string &filename, Args &&... args) {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Could not open file " + filename);
        std::string content{std::istreambuf_iterator(file), std::istreambuf_iterator<char>()};
        file.close();
        return T(content, std::forward<Args>(args)...);
    }
}

#endif //nethttp_http_body
