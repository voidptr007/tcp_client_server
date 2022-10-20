#ifndef _TCPCLIENT_HPP_
#define _TCPCLIENT_HPP_

#include "socket.hpp"

namespace Network
{
    class TcpClient
    {
    public:
        /// @brief creates a socket and binds it to address:port
        /// @param localPort local port to bind
        /// @param localAddr local adress to bind (optional)
        /// @return true on success
        bool create(const uint16_t &localPort = 0, const std::string &localAddr = "");

        /// @brief sends data to server
        /// @param data data to send
        /// @param dataLen size of data to send
        /// @return number of bytes sent
        ssize_t send(const void *const data, const uint32_t &dataLen);

        /// @brief receive data from server
        /// @param data buffer to hold received data, caller allocates buffer memory
        /// @param dataLen size of buffer
        /// @return number of bytes received
        ssize_t receive(void *const data, const uint32_t &dataLen);

        /// @brief connect to server
        /// @param serverport server port
        /// @param serveraddr server address
        /// @return
        bool connect(const uint16_t &serverport, const std::string &serveraddr);

        /// @brief closes socket
        void close();

        TcpClient() {}
        TcpClient(const TcpClient &other) = default;
        TcpClient &operator=(const TcpClient &other) = default;
        TcpClient(TcpClient &&other) = default;
        TcpClient &operator=(TcpClient &&other) = default;
        virtual ~TcpClient();

    private:
        Socket cliskt;
    };
}
#endif
