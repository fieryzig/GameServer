/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 07:44:55 PM PST
 * File Name : BaseServer.cpp
 */

#include "BaseServer.h"

#include <iostream>

namespace fieryzig{

    void BaseServer::InitServer()
    {
        epfd = epoll_create(256);
        listenfd = socket(AF_INET, SOCK_STREAM, 0);

        ev.data.fd = listenfd;
        ev.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

        memset(&serveraddr, 0, sizeof(serveraddr));
        inet_aton(config->ip.c_str(), &(serveraddr.sin_addr));
        serveraddr.sin_port = htons(config->port);
        
        SetSocketOpt(listenfd);

        if (bind(listenfd, (sockaddr*)&serveraddr, sizeof(struct sockaddr)) == -1) {
            cout << "bind failed" << endl;
            exit(1);
        }
        listen(listenfd, 10);
    }

    

    
    
}// end of namespace
