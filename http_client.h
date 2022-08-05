#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <memory>
#include <mutex>
#include <functional>
#include "http_request.h"
#include "http_response.h"
#pragma comment(lib, "ws2_32.lib")

typedef std::function<void(HttpRequest::ptr)> SetRequest;

class HttpClient
{
public:
    static HttpClient *getInstance(std::string ip = "192.168.1.220", int port = 12345);

    bool Get(std::string uri, SetRequest func);

    bool Put(std::string uri, SetRequest func);

private:
    HttpClient(std::string ip, int port, int fd);

    bool excute(std::string &uri, SetRequest &func, HTTP_REQ_TYPE &type);

    bool sendRequest(HTTP_REQ_TYPE &type);

    bool getResponse();

    int readLine(std::string &line, const int len);

    void parseLine(const std::string &line, std::string &key, std::string &value);

public:
    HttpRequest::ptr m_httpRequest;
    HttpResponse::ptr m_httpResponse;
    int m_fd;

private:
    static HttpClient *m_singleObject;
    static std::mutex *m_mutex;
};

#endif // HTTP_CLIENT_H