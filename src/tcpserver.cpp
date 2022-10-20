#include <unistd.h>
#include <memory.h>
#include "tcpserver.hpp"

namespace Network
{
    bool TcpServer::create(const uint16_t &localPort, const std::string &localAddr)
    {
        return srvskt.create(SOCK_STREAM, localPort, localAddr);
    }

    bool TcpServer::start_server(funcReceiveCallback callback, void *data, const uint32_t &dataLen)
    {
        int32_t sfd = srvskt.get_sockfd();
        if (sfd <= 0 || dataLen == 0 || data == nullptr || callback == nullptr)
        {
            fprintf(stderr, "invalid arguments, tcp server could not be started\n");
            return false;
        }

        ::listen(sfd, MAX_CLIENTS);

        epollfd = epoll_create1(0);
        if (epollfd == -1)
        {
            perror("epoll_create1");
            return false;
        }
        // epoll needs it to be non-blocking
        Socket::set_nonblocking(sfd);
        evtInterest.data.fd = sfd;
        evtInterest.events = EPOLLIN;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sfd, &evtInterest) == -1)
        {
            perror("epoll_ctl: server socket could not be added to epoll");
            return false;
        }

        while (true)
        {
            int32_t nfds = epoll_wait(epollfd, evtResponse.data(), MAX_CLIENTS, -1);
            for (int i = 0; i < nfds; ++i)
            {
                int32_t fd = evtResponse[i].data.fd;
                if (!(evtResponse[i].events & EPOLLIN))
                {
                    fprintf(stderr, "epoll event error\n");
                    ::close(fd);
                    continue;
                }
                else if (fd == sfd)
                {
                    sockaddr_in client{0};
                    socklen_t len = sizeof(client);
                    memset(&client, 0, len);
                    int32_t cfd = ::accept(sfd, (sockaddr *)&client, &len);
                    if (cfd <= 0)
                    {
                        perror("server:accept failed");
                        continue;
                    }
                    evtInterest.data.fd = cfd;
                    evtInterest.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, cfd, &evtInterest) == -1)
                    {
                        perror("epoll_ctl: client socket could not be added to epoll");
                        continue;
                    }
                    std::string cIp = std::string(inet_ntoa(client.sin_addr));
                    uint16_t cPort = ntohs(client.sin_port);
                    cliMgr.add(Socket(cfd, cPort, cIp));
                    Socket::set_nonblocking(cfd);
                    fprintf(stderr, "(+) client (%d). active clients# %lu\n", cfd, cliMgr.get_clientsockets().size());
                }
                else
                {
                    memset(data, 0, sizeof(dataLen));
                    ssize_t recvsize = ::recv(fd, data, dataLen, 0);
                    if (recvsize > 0)
                    {
                        callback(data, recvsize, cliMgr.get_client(fd));
                    }
                    else
                    {
                        cliMgr.remove(fd);
                        fprintf(stderr, "(-) client (%d). active clients# %ld\n", fd, cliMgr.get_clientsockets().size());
                    }
                }
            }
        }
        return true;
    }

    ssize_t TcpServer::send(const void *const data, const uint32_t &dataLen, const std::string &clientAddr, const uint16_t &clientPort)
    {
        int32_t cfd = cliMgr.get_sockfd(clientAddr, clientPort);
        return ::send(cfd, data, dataLen, 0);
    }

    const std::vector<Socket> &TcpServer::get_clients()
    {
        return cliMgr.get_clientsockets();
    }

    void TcpServer::close()
    {
        srvskt.close();
        cliMgr.clear();
        epollfd = -1;
        evtInterest.data.fd = 0;
        evtInterest.events = 0;
    }

    TcpServer::~TcpServer()
    {
        this->close();
    }
}