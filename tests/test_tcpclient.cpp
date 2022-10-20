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
