/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 08:18:24 AM PST
 * File Name : main.cpp
 */

#include "Const.h"
#include "Client/Client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>

#include <iostream>
#include <thread>

using namespace std;

using namespace fieryzig;

int fd;
struct sockaddr_in addr;
Client *client = nullptr;

void init()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(addr.sin_addr));
    addr.sin_port = htons(7788);
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(sockaddr));
    client = new Client(fd);
}

void recv_thread()
{
    PacketInfo pi;
    string content;
    for (;;) {
        int ret = client->Recv();
        bool flag = client->hasMsg(&pi, content);
        if (flag) {
            cout << "Recv: ";
            cout << content << endl;
        }
    }
}

int main()
{
    init();
    Client* client = new Client(fd);

    PacketInfo pi;
    pi.srcType = ServerType::CLIENT;
    pi.time = time(NULL);
    pi.uid = -1;
    pi.svrid = 0;
    pi.clifd = -1;
    pi.contentLen = 5;
    string content = "Login";

    client->Send(pi, content);
    
    thread recvThread(recv_thread);

    for(;;)
    {
        cin >> content;
        pi.time = time(NULL);
        pi.contentLen = content.length();
        client->Send(pi, content);
        if (content == "Logout") {
            close(client->fd);
            return 0;
        }
    }

    recvThread.join();

    return 0;
    
}
