#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <string>
#include <iostream>
#include <map>
#include <memory>

enum HTTP_REQ_TYPE
{
    HTTP_DEFAULT,
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE
};

enum HTTP_STATUS
{
    HTTP_STATUS_DEFAULT,
    HTTP_STATUS_CONNECT_FAILED,
    HTTP_STATUS_CONNECT_SUCCESS,
    HTTP_STATUS_CONNECT_STOP,
    HTTP_STATUS_CONNECT_CLOSE,
    HTTP_STATUS_IO_ERROR,
    HTTP_STATUS_PARSE_URL_FAILED
};

//////////////////////////////////////////////////////////////////////////
#define HTTP_REQUEST_GET "GET"
#define HTTP_REQUEST_POST "POST"
#define HTTP_REQUEST_PUT "PUT"
#define HTTP_REQUEST_DELETE "DELETE"
//////////////////////////////////////////////////////////////////////////
#define HTTP_VERSION_1_1 "HTTP/1.1"
//////////////////////////////////////////////////////////////////////////
#define HTTP_HEAD_HOST "Host"
#define HTTP_HEAD_ACCEPT "Accept"
#define HTTP_HEAD_REFERER "Referer"
#define HTTP_HEAD_ACCEPT_LANGUAGE "Accept-Language"
#define HTTP_HEAD_ACCEPT_ENCODING "Accept-Encoding"
#define HTTP_HEAD_ACCEPT_CHARSET "Accept-Charset"
#define HTTP_HEAD_ACCEPT_RANGES "Accept-Ranges"
#define HTTP_HEAD_USER_AGENT "User-Agent"
#define HTTP_HEAD_COOKIE "Cookie"
#define HTTP_HEAD_CONTENT_TYPE "Content-Type"
#define HTTP_HEAD_DATE "Date"
#define HTTP_HEAD_CHARSET "utf-8"
#define HTTP_HEAD_CONTENT_LENGTH "Content-Length"
#define HTTP_HEAD_DATE "Date"
#define HTTP_HEAD_KEEP_ALIVE "Keep-Alive"
#define HTTP_HEAD_CONNECTION "Connection"
#define HTTP_HEAD_SERVER "Server"
#define HTTP_HEAD_LOCATION "Location"
#define HTTP_HEAD_SET_COOKIE "Set-Cookie"
#define HTTP_HEAD_CONTENT_ENCODING "Content-Encoding"
#define HTTP_HEAD_TRANSFER_ENCODING "Transfer-Encoding"

#define HTTP_HEAD_CONTENT_DISPOSITION "Content-Disposition"
#define HTTP_HEAD_CONTENT_TRANSFER_ENCODING "Content-Transfer-Encoding"

class HttpRequest
{
public:
    std::string m_version;
    std::string m_uri;
    std::string m_host;
    int m_port;
    std::string m_body;
    std::map<std::string, std::string> m_headMap;

public:
    using ptr = std::shared_ptr<HttpRequest>;

    HttpRequest();

    void setHead(const std::string &key, const std::string &value);

    void reset();

    std::string toString(HTTP_REQ_TYPE &type);
};
#endif