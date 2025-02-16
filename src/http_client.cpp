#include "nethttp/http_client.hpp"
#include "netsock/tcp_stream.hpp"

namespace nethttp {
    http_client::http_client()= default;

    http_response http_client::send_request(const http_method method, const url &url, const http_body &body,
        const http_headers &headers) {
        http_request request(method);
        request.set_headers(headers);
        request.set_body(body);
        for (const auto &[name, values] : _default_headers) {
            if (!request.has_header(name))
                request.add_header(name, values);
        }
        request.set_path(url.full_path());
        if (!request.has_header("Host"))
            request.add_header("Host", url.host());
        netsock::tcp_client client;
        client.reuse_address(true);
        netsock::tcp_stream stream(client);
        const auto addresses = netsock::dns::resolve(url.host());
        if (addresses.empty())
            throw http_request_error("Error resolving url (perhaps you put the wrong host)");
        for (const auto &address : addresses) {
            try {
                client.connect(address & (url.port() == 0 ? 80 : url.port()));
                if (client.connected())
                    break;
            } catch (const netsock::socket_error &) {

            }
        }
        if (!client.connected()) {
            throw http_request_error("Error connecting (perhaps you put the wrong address)");
        }
        http_response_message response;
        if (to_stream(request.message(), stream).fail())
            throw http_request_error("Error writing request to stream (perhaps the library messed up)");
        stream.flush();
        if (from_stream(response, stream).fail())
            throw http_request_error("Error reading response from stream (perhaps the library messed up)");
        return response;
    }

    http_response http_client::get_request(const url &url, const http_headers &headers) {
        return send_request(GET, url, empty_body(), headers);
    }

    http_response http_client::post_request(const url &url, const http_body &body, const http_headers &headers) {
        return send_request(POST, url, body, headers);
    }

    void http_client::add_default_header(std::string name, http_header_values values) {
        _default_headers.add(std::move(name), std::move(values));
    }

    void http_client::set_default_headers(const http_headers &default_headers) {
        _default_headers = default_headers;
    }

    const http_headers & http_client::default_headers() const {
        return _default_headers;
    }
}
