/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 03:34:47 AM PST
 * File Name : Const.h
 */

#ifndef CONST_H
#define CONST_H

namespace fieryzig {   

const int EPOLL_SIZE = 20;
const int BUFSIZE = 4096;

enum ResultStatus {
    SUCCESS,
    FAILED,
};
using RS = ResultStatus;

enum ServerType {
    CLIENT,
    GATEWAY,
    LOGIC,
};

struct PacketInfo
{
    ServerType srcType;
    int time;    
    int uid;
    int svrid;
    int clifd;
    int contentLen;
    // ...
    // perhaps, it's ok to expand here without modifying other codes;
};

}

#endif
