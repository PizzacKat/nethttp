#ifndef nethttp_http_headers
#define nethttp_http_headers

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace nethttp {
    class http_header_values {
    public:
        http_header_values()= default;

        http_header_values(std::string value): _values{std::move(value)} {

        }

        http_header_values(const char *value): _values{value} {

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

        [[nodiscard]] bool contains(const std::string &name) const {
            return std::ranges::find(_values, name) != _values.end();
        }

        void add(std::string value) {
            _values.emplace_back(std::move(value));
        }

        [[nodiscard]] std::size_t count() const {
            return _values.size();
        }

        [[nodiscard]] const std::string &get(const std::size_t i = 0) const {
            return _values.at(i);
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

        void add_or_set(std::string name, http_header_values values) {
            if (has(name))
                get(name) = std::move(values);
            else
                add(std::move(name), std::move(values));
        }

        void add(std::string name, http_header_values values) {
            _map.emplace(std::move(name), std::move(values));
        }

        void clear() {
            _map.clear();
        }

        void remove(const std::string &name) {
            _map.erase(name);
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

    inline std::ostream &to_stream(const http_headers &headers, std::ostream &os) {
        for (const auto &[name, values] : headers) {
            os << name << ": ";
            for (std::size_t i = 0; i < values.count(); i++) {
                os << values.get(i);
                if (i != values.count() - 1)
                    os << ", ";
            }
            os << "\r\n";
        }
        os << "\r\n";
        return os;
    }

    inline std::istream &from_stream(http_headers &headers, std::istream &is) {
        while (is.peek() != '\r') {
            std::string key;
            if (!std::getline(is, key, ':')) {
                is.setstate(std::ios::failbit);
                return is;
            }
            http_header_values values;
            if (is.peek() == '\r') {
                is.setstate(std::ios::failbit);
                return is;
            }
            std::string values_string;
            is >> std::ws;
            if (!std::getline(is, values_string, '\r')) {
                is.setstate(std::ios::failbit);
                return is;
            }
            if (values_string.find(',') == std::string::npos)
                values += values_string;
            else {
                std::string value;
                std::stringstream ss(values_string);
                while (std::getline(ss, value, ',')) {
                    values += value;
                }
            }
            if (is.get() != '\n') {
                is.setstate(std::ios::failbit);
                return is;
            }
            headers.add(key, values);
        }
        if (is.get() != '\r' || is.get() != '\n') {
            is.setstate(std::ios::failbit);
            return is;
        }
        return is;
    }

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
