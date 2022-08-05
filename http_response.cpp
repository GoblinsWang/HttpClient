#include "http_response.h"

HttpResponse::HttpResponse()
{
    m_code = HTTPRESPONSE_000;
    m_body = "";
}

void HttpResponse::setHead(const std::string &key, std::string &value)
{
    m_headMap[key] = value;
}

void HttpResponse::reset()
{
    m_code = HTTPRESPONSE_000;
    m_body.clear();
    m_headMap.clear();
}