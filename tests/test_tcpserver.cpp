#include <thread>
#include "tcpserver.hpp"

Network::TcpServer tcpserver;
void MsgRecvCallback(void *const data, ssize_t &dataSize, const Network::Socket &client)
{
    tcpserver.send(data, dataSize, client.get_addr(), client.get_port());
    return;
}

int main(int argc, char *argv[])
{
    char buffer[64];
    int serverport = 9000;
    const std::string serverip = "localhost";
    if (argc > 1 && atoi(argv[1]) > 0)
    {
        serverport = atoi(argv[1]);
    }

    fprintf(stderr, "starting tcp server @%s:%d\n", serverip.c_str(), serverport);
    if (tcpserver.create(serverport, serverip))
    {
        std::thread tServer = std::thread(&Network::TcpServer::start_server, &tcpserver, &MsgRecvCallback, buffer, sizeof(buffer));
        tServer.join();
    }
    return 0;
}
