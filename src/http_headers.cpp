#include "nethttp/http_headers.hpp"
#include <iostream>
#include <sstream>

namespace nethttp {
    http_header_values::http_header_values()= default;

    http_header_values::http_header_values(std::string value): _values{std::move(value)} {

    }

    http_header_values::http_header_values(const char *value): _values{value} {

    }

    http_header_values::http_header_values(std::vector<std::string> values): _values{std::move(values)} {

    }

    http_header_values http_header_values::operator+(const http_header_values &other) const {
        auto copy = *this;
        copy._values.insert(copy._values.end(), other._values.begin(), other._values.end());
        return copy;
    }

    http_header_values & http_header_values::operator+=(const http_header_values &other) {
        _values.insert(_values.end(), other._values.begin(), other._values.end());
        return *this;
    }

    bool http_header_values::contains(const std::string &name) const {
        return std::ranges::find(_values, name) != _values.end();
    }

    void http_header_values::add(std::string value) {
        _values.emplace_back(std::move(value));
    }

    std::size_t http_header_values::count() const {
        return _values.size();
    }

    const std::string & http_header_values::get(const std::size_t i) const {
        return _values.at(i);
    }

    std::vector<std::string>::iterator http_header_values::begin() {
        return _values.begin();
    }

    std::vector<std::string>::iterator http_header_values::end() {
        return _values.end();
    }

    std::vector<std::string>::const_iterator http_header_values::begin() const {
        return _values.begin();
    }

    std::vector<std::string>::const_iterator http_header_values::end() const {
        return _values.end();
    }

    void http_headers::add_or_set(std::string name, http_header_values values) {
        if (has(name))
            get(name) = std::move(values);
        else
            add(std::move(name), std::move(values));
    }

    void http_headers::add(std::string name, http_header_values values) {
        _map.emplace(std::move(name), std::move(values));
    }

    void http_headers::clear() {
        _map.clear();
    }

    void http_headers::remove(const std::string &name) {
        _map.erase(name);
    }

    bool http_headers::has(const std::string &key) const {
        return _map.contains(key);
    }

    const http_header_values & http_headers::get(const std::string &key) const {
        return _map.at(key);
    }

    const http_header_values & http_headers::operator[](const std::string &key) const {
        return _map.at(key);
    }

    http_header_values & http_headers::get(const std::string &key) {
        return _map.at(key);
    }

    http_header_values & http_headers::operator[](const std::string &key) {
        return _map.at(key);
    }

    std::size_t http_headers::size() const {
        return _map.size();
    }

    std::unordered_map<std::string, http_header_values>::iterator http_headers::begin() {
        return _map.begin();
    }

    std::unordered_map<std::string, http_header_values>::iterator http_headers::end() {
        return _map.end();
    }

    std::unordered_map<std::string, http_header_values>::const_iterator http_headers::begin() const {
        return _map.begin();
    }

    std::unordered_map<std::string, http_header_values>::const_iterator http_headers::end() const {
        return _map.end();
    }

    std::ostream & to_stream(const http_headers &headers, std::ostream &os) {
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

    std::istream & from_stream(http_headers &headers, std::istream &is) {
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
}
