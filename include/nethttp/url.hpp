#ifndef nethttp_url
#define nethttp_url

#include <map>
#include <stdexcept>
#include <string>

namespace nethttp {
    class url {
    public:
        url()= default;
        url(const std::string &url) {
            *this = parse(url);
        }
        url(const char *url) {
            *this = parse(url);
        }

        static url parse(const std::string &url) {
            nethttp::url parsed;
            const char *c = url.c_str();
            const char *beg = c;
            while (*c != ':' && *c != 0) c++;
            if (*c == 0)
                throw std::invalid_argument("invalid url");
            parsed._protocol = std::string(beg, c);
            c++;
            if (*c++ != '/' || *c++ != '/')
                throw std::invalid_argument("invalid url");
            beg = c;
            while (*c != ':' && *c != '/' && *c != 0) c++;
            parsed._host = std::string(beg, c);
            if (*c == 0)
                return parsed;
            if (*c == ':') {
                c++;
                beg = c;
                while (*c != '/' && *c != 0) c++;
                parsed._port = std::stoi(std::string(beg, c));
            }
            if (*c == 0)
                return parsed;
            beg = c;
            c++;
            if (*c == 0)
                return parsed;
            while (*c != '?' && *c != 0) c++;
            if (*c == 0)
                return parsed;
            c++;
            parsed._path = std::string(beg, c);
            while (*c != 0) {
                beg = c;
                while (*c != '=' && *c != 0) c++;
                if (*c == 0)
                    throw std::invalid_argument("invalid url");
                std::string key(beg, c);
                c++;
                while (*c != '&' && *c != 0) c++;
                std::string value(beg, c);
                parsed._query.emplace(std::move(key), std::move(value));
                if (*c == 0)
                    break;
                c++;
            }
            return parsed;
        }

        [[nodiscard]] const std::string &protocol() const {
            return _protocol;
        }

        [[nodiscard]] const std::string &host() const {
            return _host;
        }

        [[nodiscard]] std::uint16_t port() const {
            return _port;
        }

        [[nodiscard]] const std::string &path() const {
            return _path;
        }

        [[nodiscard]] const std::map<std::string, std::string> &query() const {
            return _query;
        }

        [[nodiscard]] std::string full_path() const {
            if (_query.empty())
                return _path;
            std::string path = _path;
            path += '?';
            for (const auto &[name, value] : _query) {
                path += name;
                path += '=';
                path += value;
                path += '&';
            }
            return path.substr(0, path.size() - 1);
        }
    private:
        std::string _protocol{};
        std::string _host{};
        std::uint16_t _port{0};
        std::string _path{"/"};
        std::map<std::string, std::string> _query{};
    };
}

#endif //nethttp_url
