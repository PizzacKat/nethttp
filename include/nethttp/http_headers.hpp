#ifndef nethttp_http_headers
#define nethttp_http_headers

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace nethttp {
    class http_header_values {
    public:
        http_header_values();
        http_header_values(std::string value);
        http_header_values(const char *value);
        http_header_values(std::vector<std::string> values);

        http_header_values operator+(const http_header_values &other) const;
        http_header_values &operator+=(const http_header_values &other);

        [[nodiscard]] bool contains(const std::string &name) const;

        void add(std::string value);

        [[nodiscard]] std::size_t count() const;

        [[nodiscard]] const std::string &get(std::size_t i = 0) const;

        [[nodiscard]] std::vector<std::string>::iterator begin();
        [[nodiscard]] std::vector<std::string>::iterator end();
        [[nodiscard]] std::vector<std::string>::const_iterator begin() const;
        [[nodiscard]] std::vector<std::string>::const_iterator end() const;

    private:
        std::vector<std::string> _values;
    };

    class http_headers {
    public:
        http_headers()= default;

        void add_or_set(std::string name, http_header_values values);
        void add(std::string name, http_header_values values);
        void clear();
        void remove(const std::string &name);

        [[nodiscard]] bool has(const std::string &key) const;

        [[nodiscard]] const http_header_values &get(const std::string &key) const;
        [[nodiscard]] const http_header_values &operator[](const std::string &key) const;
        [[nodiscard]] http_header_values &get(const std::string &key);
        [[nodiscard]] http_header_values &operator[](const std::string &key);

        [[nodiscard]] std::size_t size() const;

        [[nodiscard]] std::unordered_map<std::string, http_header_values>::iterator begin();
        [[nodiscard]] std::unordered_map<std::string, http_header_values>::iterator end();
        [[nodiscard]] std::unordered_map<std::string, http_header_values>::const_iterator begin() const;
        [[nodiscard]] std::unordered_map<std::string, http_header_values>::const_iterator end() const;

    private:
        std::unordered_map<std::string, http_header_values> _map;
    };

    inline std::ostream &to_stream(const http_headers &headers, std::ostream &os);
    inline std::istream &from_stream(http_headers &headers, std::istream &is);

    // namespace content_headers {
    //     inline const http_header_values &allow(const http_headers &headers) {
    //         return headers["Allow"];
    //     }
    //
    //     inline void allow(http_headers &headers, const http_header_values &allow) {
    //         if (!headers.has("Allow")) {
    //             headers.add("Allow", allow);
    //             return;
    //         }
    //         headers["Allow"] = allow;
    //     }
    //
    //     inline const content_disposition_value &content_disposition(const http_headers &headers) {
    //         return headers["Allow"];
    //     }
    // }
}

#endif //nethttp_http_headers
