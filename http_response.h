#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include <string>
#include <iostream>
#include <memory>
#include <map>

enum HTTPRESPONSE_CODE
{
    HTTPRESPONSE_000 = 0,
    HTTPRESPONSE_200 = 200,
    HTTPRESPONSE_204 = 204,
    HTTPRESPONSE_206 = 206,
    HTTPRESPONSE_301 = 301,
    HTTPRESPONSE_302 = 302,
    HTTPRESPONSE_400 = 400,
    HTTPRESPONSE_403 = 403,
    HTTPRESPONSE_404 = 404,
    HTTPRESPONSE_413 = 413,
    HTTPRESPONSE_500 = 500,
};

class HttpResponse
{
public:
    using ptr = std::shared_ptr<HttpResponse>;

    HttpResponse();

    void setHead(const std::string &key, std::string &value);

    void reset();

public:
    HTTPRESPONSE_CODE m_code;

    std::string m_body;

    std::map<std::string, std::string> m_headMap;
};

#endif