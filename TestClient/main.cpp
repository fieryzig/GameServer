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

using namespace std;

using namespace fieryzig;

int main()
{
    int fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(addr.sin_addr));
    addr.sin_port = htons(7788);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(sockaddr));
    
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

    return 0;
    
}
