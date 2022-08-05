#include "http_client.h"

bool initEnv();

HttpClient *HttpClient::m_singleObject = nullptr;
std::mutex *HttpClient::m_mutex = new std::mutex();

HttpClient *HttpClient::getInstance(std::string ip, int port)
{
    m_mutex->lock();
    do
    {
        if (m_singleObject != nullptr || !initEnv())
            break;
        // ------------------create socket--------------
        SOCKET sock = INVALID_SOCKET;
        sock = socket(2, SOCK_STREAM, IPPROTO_TCP);
        if (INVALID_SOCKET == sock)
        {
            std::cout << "socket error" << WSAGetLastError() << std::endl;
            WSACleanup();
            break;
        }
        else
        {
            std::cout << "socket success" << std::endl;
        }
        sockaddr_in soserver;
        soserver.sin_family = AF_INET;
        soserver.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
        soserver.sin_port = htons(port);

        // -----------------connect server----------------
        int rt = connect(sock, (SOCKADDR *)&soserver, sizeof(soserver));
        if (rt == 0)
        {
            std::cout << "connect success" << std::endl;
        }
        else
        {
            std::cout << "connect error: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            break;
        }

        // -----------------init httpclient------------------
        m_singleObject = new HttpClient(ip, port, sock);

    } while (false);
    m_mutex->unlock();

    return m_singleObject;
}

HttpClient::HttpClient(std::string ip, int port, int fd)
    : m_fd(fd)
{
    m_httpRequest = std::make_shared<HttpRequest>();
    m_httpResponse = std::make_shared<HttpResponse>();
}

bool HttpClient::Get(std::string uri, SetRequest func)
{
    HTTP_REQ_TYPE type = HTTP_GET;
    return excute(uri, func, type);
}

bool HttpClient::Put(std::string uri, SetRequest func)
{
    HTTP_REQ_TYPE type = HTTP_PUT;
    return excute(uri, func, type);
}

bool HttpClient::excute(std::string &uri, SetRequest &func, HTTP_REQ_TYPE &type)
{
    do
    {
        m_httpRequest->reset();
        m_httpRequest->m_uri = std::move(uri);
        // set by
        func(m_httpRequest);
        // send Get resquest
        if (!sendRequest(type))
        {
            break;
        }
        std::cout << "send resquest success" << std::endl;
        // recv http response
        if (!getResponse())
        {
            std::cout << "getResponse failed" << std::endl;
            break;
        }
        std::cout << "getResponse success" << std::endl;
        return true;
    } while (false);

    return false;
}

bool HttpClient::sendRequest(HTTP_REQ_TYPE &type)
{
    auto strRequest = m_httpRequest->toString(type);

    bool bRet = true;
    std::string data = strRequest;
    while (data.length() > 0)
    {
        int nSend = ::send(m_fd, data.c_str(), data.length(), 0);
        if (nSend < 0)
        {
            bRet = false;
            break;
        }
        else if (nSend > 0)
        {
            data = data.substr(nSend, data.length() - nSend);
        }
    }
    return bRet;
}

bool HttpClient::getResponse()
{
    bool res = true;
    m_httpResponse->reset();
    for (;;)
    {
        std::string line;
        int ret = readLine(line, 1024);
        // std::cout << line << std::endl;
        if (ret <= 0)
        {
            res = false; // closed
            break;
        }
        // find empty line
        if (line.size() == strlen("\r\n") && line.data() == strstr(line.data(), "\r\n"))
        {
            ret = readLine(line, 1024); // get response body
            if (ret > 0)
                m_httpResponse->m_body = line.substr(0, line.length() - 2);
            break;
        }
        else
        {
            std::string strKey, strValue;
            parseLine(line, strKey, strValue);
            if (strKey.data() == strstr(strKey.data(), "HTTP"))
            {
                size_t pos = strValue.find(" ");
                if (pos != strValue.npos)
                {
                    std::string strCode = strValue.substr(0, pos);
                    // std::cout << "strCode:" << strCode << std::endl;
                    m_httpResponse->m_code = (HTTPRESPONSE_CODE)atoi(strCode.c_str());
                }
                continue;
            }
            m_httpResponse->setHead(strKey, strValue);
        }
    }
    return true;
}

int HttpClient::readLine(std::string &line, const int len)
{
    line.clear();
    char ch = 0;
    uint32_t nSize = 0;
    do
    {
        if (len > 0 && nSize >= len)
        {
            break;
        }
        int ret = ::recv(m_fd, &ch, 1, 0);
        if (ret == 1)
        {
            nSize++;
            line.append(1, ch);
        }
        else if (ret == 0)
        {
            break;
        }
        else
        {
            nSize = (nSize == 0 ? -1 : nSize);
            break;
        }
    } while (ch != '\n');
    return nSize;
}

void HttpClient::parseLine(const std::string &line, std::string &key, std::string &value)
{
    std::string delimiters = "\r\n";
    size_t pos1 = line.find_first_not_of(delimiters);
    size_t pos2 = line.find_last_not_of(delimiters);
    std::string strLine = line;
    if (std::string::npos != pos1 && std::string::npos != pos2)
    {
        strLine = line.substr(pos1, pos2 - pos1 + 1);
    }
    char *start = (char *)strLine.data();
    if (strstr(strLine.data(), "HTTP") == strLine.data())
    {
        char *pos = strstr(start, " ");
        if (pos)
        {
            key.append(start, pos - start);
            pos += strlen(" ");
            value.append(pos);
        }
    }
    else
    {
        char *pos = strstr(start, ": ");
        if (pos)
        {
            key.append(start, pos - start);
            pos += strlen(": ");
            value.append(pos);
        }
    }
}

bool initEnv()
{
    // -------------------- init env -------------------
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 0;
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 0;
    }
    else
    {
        printf("The Winsock 2.2 dll was found okay\n");
        return 1;
    }
}