#ifndef nethttp_http_headers
#define nethttp_http_headers

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

namespace nethttp {
    class http_header_values {
    public:
        http_header_values()= default;

        http_header_values(std::string value): _values{std::move(value)} {

        }

        http_header_values(std::vector<std::string> values): _values{std::move(values)} {

        }

        http_header_values operator+(const http_header_values &other) const {
            auto copy = *this;
            copy._values.insert(copy._values.end(), other._values.begin(), other._values.end());
            return copy;
        }

        http_header_values &operator+=(const http_header_values &other) {
            _values.insert(_values.end(), other._values.begin(), other._values.end());
            return *this;
        }

        void add(std::string value) {
            _values.emplace_back(std::move(value));
        }

        [[nodiscard]] std::size_t count() const {
            return _values.size();
        }

        [[nodiscard]] std::vector<std::string>::iterator begin() {
            return _values.begin();
        }

        [[nodiscard]] std::vector<std::string>::iterator end() {
            return _values.end();
        }

        [[nodiscard]] std::vector<std::string>::const_iterator begin() const {
            return _values.begin();
        }

        [[nodiscard]] std::vector<std::string>::const_iterator end() const {
            return _values.end();
        }
    private:
        std::vector<std::string> _values;
    };

    class http_headers {
    public:
        http_headers()= default;

        void add(std::string name, http_header_values values) {
            _map.emplace(std::move(name), std::move(values));
        }

        void clear() {
            _map.clear();
        }

        [[nodiscard]] bool has(const std::string &key) const {
            return _map.contains(key);
        }

        [[nodiscard]] const http_header_values &get(const std::string &key) const {
            return _map.at(key);
        }

        [[nodiscard]] const http_header_values &operator[](const std::string &key) const {
            return _map.at(key);
        }

        [[nodiscard]] http_header_values &get(const std::string &key) {
            return _map.at(key);
        }

        [[nodiscard]] http_header_values &operator[](const std::string &key) {
            return _map.at(key);
        }

        [[nodiscard]] std::size_t size() const {
            return _map.size();
        }

        [[nodiscard]] std::unordered_map<std::string, http_header_values>::iterator begin() {
            return _map.begin();
        }

        [[nodiscard]] std::unordered_map<std::string, http_header_values>::iterator end() {
            return _map.end();
        }

        [[nodiscard]] std::unordered_map<std::string, http_header_values>::const_iterator begin() const {
            return _map.begin();
        }

        [[nodiscard]] std::unordered_map<std::string, http_header_values>::const_iterator end() const {
            return _map.end();
        }
    private:
        std::unordered_map<std::string, http_header_values> _map;
    };

    namespace content_headers {
        inline const http_header_values &allow(const http_headers &headers) {
            return headers["Allow"];
        }

        inline void allow(http_headers &headers, const http_header_values &allow) {
            if (!headers.has("Allow")) {
                headers.add("Allow", allow);
                return;
            }
            headers["Allow"] = allow;
        }

        struct content_disposition_value {
            content_disposition_value() = default;

            std::chrono::milliseconds creation_time;
            std::chrono::milliseconds modification_time;
            std::string filename;
            std::string filename_star;
            std::string name;

        };

        inline const content_disposition_value &content_disposition(const http_headers &headers) {
            return headers["Allow"];
        }
    }
}

#endif //nethttp_http_headers
