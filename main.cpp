#include "http_client.h"

int main()
{
    // 初始化一个单例模式的http client.
    HttpClient *client = HttpClient::getInstance();

    // 登入请求
    client->Get("/login", [](HttpRequest::ptr req)
                {
                    req->setHead("userId", "zhangsan");
                    req->setHead("mId", "one"); });

    std::cout << "m_code:" << client->m_httpResponse->m_code << std::endl;

    // 获取对方IP
    client->Get("/route", [](HttpRequest::ptr req)
                {   
                    req->setHead("userId", "zhangsan");
                    req->setHead("mId", "one"); });

    std::cout << "m_code:" << client->m_httpResponse->m_code << std::endl;
    std::cout << "ip:" << client->m_httpResponse->m_headMap["ip"] << std::endl;
    std::cout << "port:" << client->m_httpResponse->m_headMap["port"] << std::endl;

    // 探针保活
    client->Get("/ping", [](HttpRequest::ptr req)
                {
                    req->setHead("userId", "zhangsan");
                    req->setHead("mId", "one"); });
    std::cout << "m_code:" << client->m_httpResponse->m_code << std::endl;

    closesocket(client->m_fd);
    // system("pause");

    return 0;
}