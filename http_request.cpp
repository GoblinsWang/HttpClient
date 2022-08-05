#include "http_request.h"
#include <sstream>
HttpRequest::HttpRequest()
    : m_version(HTTP_VERSION_1_1), m_uri("/")
{

    m_headMap[HTTP_HEAD_ACCEPT] = "*/*";
    m_headMap[HTTP_HEAD_ACCEPT_LANGUAGE] = "zh-CN";
    m_headMap[HTTP_HEAD_USER_AGENT] = "SDK C++ http client";
    m_headMap[HTTP_HEAD_CONNECTION] = "Keep-Alive";
}

void HttpRequest::setHead(const std::string &key, const std::string &value)
{
    m_headMap[key] = value;
}

void HttpRequest::reset()
{
    m_uri = "/";
    m_body.clear();
}

std::string HttpRequest::toString(HTTP_REQ_TYPE &type)
{
    std::string strHttpType;
    switch (type)
    {
    case HTTP_GET:
        strHttpType = HTTP_REQUEST_GET;
        break;
    case HTTP_POST:
        strHttpType = HTTP_REQUEST_POST;
        break;
    case HTTP_PUT:
        strHttpType = HTTP_REQUEST_PUT;
        break;
    case HTTP_DELETE:
        strHttpType = HTTP_REQUEST_DELETE;
        break;
    }
    std::ostringstream oss;
    oss << strHttpType << " " << m_uri << " " << m_version << "\r\n";
    oss << HTTP_HEAD_HOST << ": " << m_host;
    if (m_port != 80)
    {
        oss << ":" << m_port;
    }
    oss << "\r\n";
    for (std::map<std::string, std::string>::iterator it = m_headMap.begin(); it != m_headMap.end(); ++it)
    {
        oss << it->first << ": " << it->second << "\r\n";
    }
    // empty line
    oss << "\r\n";
    // write body
    oss << m_body;
    return oss.str();
}