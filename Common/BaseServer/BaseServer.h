/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 03:43:40 AM PST
 * File Name : BaseServer.h
 */

#ifndef BASESERVER_H
#define BASESERVER_H

#include "../Const.h"
#include "../Client/Client.h"
#include "ServerConfig.h"

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <vector>

using namespace std;

namespace fieryzig{
    
class BaseServer
{
public:
    virtual RS SetConfig(const char* configFile) = 0;
    void InitServer();

    virtual void Run() = 0;
    virtual void Dispatch(Client*, PacketInfo*, string) = 0;
    
protected:
    ServerConfig *config = nullptr;
    virtual void acceptConnect() = 0;

    
protected:
    int epfd, listenfd, connfd;
    
    struct epoll_event ev, events[EPOLL_SIZE];
    
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    inline void SetSocketOpt(int fd) {
        int flag = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    }

};
   
} //end of namespace

#endif
