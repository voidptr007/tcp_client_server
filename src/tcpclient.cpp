#include <arpa/inet.h>
#include "tcpclient.hpp"

namespace Network
{
    bool TcpClient::create(const uint16_t &localPort, const std::string &localAddr)
    {
        return cliskt.create(SOCK_STREAM, localPort, localAddr);
    }

    bool TcpClient::connect(const uint16_t &serverport, const std::string &serveraddr)
    {
        if (cliskt.get_sockfd() < 1)
        {
            return false;
        }
        int ret = -1;
        sockaddr_in sktaddr{0};
        cliskt.get_hostbyname(serveraddr, sktaddr);
        sktaddr.sin_port = htons(serverport);
        printf("connecting to server @%s(%s):%d\n", serveraddr.c_str(), inet_ntoa(sktaddr.sin_addr), ntohs(sktaddr.sin_port));
        if ((ret = ::connect(cliskt.get_sockfd(), (sockaddr *)&sktaddr, sizeof(sktaddr))) != 0)
        {
            perror("client: connect failed");
        }
        return (ret == 0) ? true : false;
    }

    ssize_t TcpClient::send(const void *const data, const uint32_t &dataLen)
    {
        return ::send(cliskt.get_sockfd(), data, dataLen, 0);
    }

    ssize_t TcpClient::receive(void *const data, const uint32_t &dataLen)
    {
        ssize_t ret = -1;
        if ((ret = ::recv(cliskt.get_sockfd(), data, dataLen, 0)) <= 0)
        {
            cliskt.close();
        }
        return ret;
    }

    void TcpClient::close()
    {
        cliskt.close();
    }

    TcpClient::~TcpClient()
    {
        this->close();
    }
}
