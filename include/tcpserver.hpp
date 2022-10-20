#ifndef _TCPSERVER_HPP_
#define _TCPSERVER_HPP_

#include <arpa/inet.h>
#include <functional>
#include <vector>
#include <sys/epoll.h>
#include "socket.hpp"
#include "clientmanager.hpp"

namespace Network
{
    static const int MAX_CLIENTS = 200;
    typedef std::function<void(void *const, ssize_t &, const Network::Socket &)> funcReceiveCallback;

    class TcpServer
    {
    public:
        /// @brief creates a tcp server socket and binds it to address:port
        /// @param localPort local port for server socket
        /// @param localAddr local address for server socket (optional)
        /// @return true, if successful
        bool create(const uint16_t &localPort, const std::string &localAddr = "");

        /// @brief starts a tcp server. keeps listening to connections and data from clients
        /// @param callback callback from caller, will be called whenever data is received from client
        /// @param data data received from client
        /// @param dataLen size of data received
        /// @return true, if successful
        bool start_server(funcReceiveCallback callback, void *data, const uint32_t &dataLen);

        /// @brief send data to client
        /// @param data data to send
        /// @param dataLen size of data to send
        /// @param cAddr client address
        /// @param cPort clent port
        /// @return number of bytes sent
        ssize_t send(const void *const data, const uint32_t &dataLen, const std::string &cAddr, const uint16_t &cPort);

        /// @brief returns all the connected clients
        /// @return vector of connect client Sockets
        const std::vector<Socket> &get_clients();

        /// @brief closes server socket
        void close();

        TcpServer() {}
        TcpServer(const TcpServer &other) = default;
        TcpServer &operator=(const TcpServer &other) = default;
        TcpServer(TcpServer &&other) = default;
        TcpServer &operator=(TcpServer &&other) = default;
        virtual ~TcpServer();

    private:
        int32_t epollfd = -1;
        struct epoll_event evtInterest;
        std::array<struct epoll_event, MAX_CLIENTS> evtResponse;
        Socket srvskt;
        ClientManager<Socket> cliMgr;
        int32_t accept_connection(int32_t &sfd);
    };
}
#endif
