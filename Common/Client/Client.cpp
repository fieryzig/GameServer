/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 09:32:00 PM PST
 * File Name : Client.cpp
 */

#include "Client.h"
#include "../Const.h"

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

namespace fieryzig{

int Client::Recv() 
{
    if (recvbuf.remain() < 4) return 0;
//    cout << "RECV: " << recvbuf.head << " " << recvbuf.tail << " " << recvbuf.remain() << endl;
//    int ret = ::read(fd, recvbuf.getTail(), recvbuf.remain());
    int ret = ::read(fd, recvbuf.getTail(), min(recvbuf.remain(),BUFSIZE-recvbuf.tail));
    debugPrint(recvbuf.buffer,4096);
    if (ret == -1) return ret;
    recvbuf.push(ret);
    cout << "#############" << recvbuf.tail << endl;
    return ret;
}

bool Client::hasMsg(PacketInfo* ppi, string &content)
{
    if (recvbuf.count < 4) return false;
    int packetLen = recvbuf.getIntFromHead();

    if (recvbuf.count < packetLen) {
        return false;
    }
    recvbuf.pop(4);
    cout << packetLen << endl;

    int pi_size = sizeof(PacketInfo);
    recvbuf.copyFromHead(ppi, pi_size);
    recvbuf.pop(pi_size);

    int contentLen = ppi->contentLen;
    char conBuf[contentLen];
    recvbuf.copyFromHead(conBuf, contentLen);
//    debugPrint(conBuf, contentLen);
    conBuf[contentLen] = '\0';
    content = conBuf;
    
    recvbuf.pop(contentLen);
    return true;
}

void Client::Send(PacketInfo pi, string content)
{
    int tot_len = sizeof(pi) + content.length() + 4;
    unsigned int tmp = htonl(tot_len); 
    sendbuf.copyIntoTail((char*)&tmp ,4);
    sendbuf.push(4);
    sendbuf.copyIntoTail(&pi, sizeof(pi));
    sendbuf.push(sizeof(pi));
    sendbuf.copyIntoTail(content.c_str(), content.length());
    sendbuf.push(content.length());
//    cout << "SEND: " << sendbuf.head << " " << sendbuf.tail << endl;
    int ret = ::write(fd, sendbuf.getHead(), sendbuf.count);
    sendbuf.pop(ret);
}

void Client::SendRemain()
{
    if (sendbuf.count == 0) return;
//    int ret = ::write(fd, sendbuf.getHead(), sendbuf.count);
    int ret = ::write(fd, sendbuf.getHead(), BUFSIZE - sendbuf.head);
    sendbuf.pop(ret);
}

void Client::debugPrint(char* ch, int sz)
{
    cout << "+++++++" << sz << "+++++++" << endl;
    for (int i = 0; i < sz; i++) {
        printf("%x",*(ch+i));
        if ( i % 64 == 63 ) puts("");
    }
    cout << endl << "----------------" << endl;
}

}
