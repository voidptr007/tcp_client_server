# Project: tcp client server
1. Code Structure & Build
2. Dependencies
3. Usage with example
4. Design Highights
5. License

# Code Structure and build & run
- include - contains all header files (hpp)
- src - contains all implementation code (cpp)
- tests - example code illustrating usage

## Dependencies
- Pthread
- cmake 3.8

## How to build & run
Create build folder in project root directory and run cmake and make commands from there. Both client and server executables will be generated in build directory. Server port is optional, default 9000 is used.
```console
tcp_client_server$ mkdir build
tcp_client_server$ cd build
tcp_client_server/build$ cmake ..
tcp_client_server/build$ make
tcp_client_server/build$ ./tcpserver <port number>
tcp_client_server/build$ ./tcpclient <server port>
```
# Usage with examples
## TcpServer
Create an object of type TcpServer, providing port number and server address (optional).
Start tcp server in a thread. Provide receive callback handler and buffer to store received data.
start server method keeps monitoring for new client connection requests and data from connected
clients. It will invoke registered callback with data when data is received from a client.
The callback will provide received data, size in bytes and client socket that sent the data.
```c++
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
```
## TcpClient
Create an object of type TcpClient with optional local port and address to use for the client.
Connect to server using server's port and address. Send and receive data.
```c++
#include <string.h>
#include "tcpclient.hpp"

int main(int argc, char *argv[])
{
    int serverport = 9000;
    const std::string serverip = "localhost";
    const std::string testdata = "Tcp client-server loopback test message!!";
    if (argc > 1 && atoi(argv[1]) > 0)
    {
        serverport = atoi(argv[1]);
    }

    Network::TcpClient tcpclient;
    if (tcpclient.create(0, "localhost") && tcpclient.connect(serverport, serverip))
    {
        if (tcpclient.send(testdata.c_str(), testdata.length()) > 0)
        {
            char datarcvd[64] = {0};
            ssize_t rcvdsize = tcpclient.receive(datarcvd, sizeof(datarcvd));
            if (rcvdsize == testdata.length() && strcmp(datarcvd, testdata.c_str()) == 0)
            {
                fprintf(stdout, "loopback test passed. recevied: %s\n", datarcvd);
            }
            else
            {
                fprintf(stdout, "loopback test failed. recevied: %s\n", datarcvd);
            }
        }
    }
    return 0;
}
```