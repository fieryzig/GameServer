/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 09:17:29 PM PST
 * File Name : Client.h
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <assert.h>
#include <arpa/inet.h>
#include <string.h>

#include "../Const.h"

using namespace std;

namespace fieryzig{
    
class Client
{
public:
    Client(int fd):fd(fd){}
    int Recv();
    bool hasMsg(PacketInfo* ppi, string &content);
    void Send(PacketInfo pi, string content);
    void SendRemain();

private:

    struct buffer_t
    {
        buffer_t():head(0),tail(0),count(0){}
        char buffer[BUFSIZE+1];
        int head, tail, count;
        inline int remain() {
            return BUFSIZE - count;
        }
        inline char* seek(int pos) {
//            assert(pos<BUFSIZE);
            return (char*)(buffer+pos);
        }
        inline char* getTail() {
            return seek(tail);
        }
        inline char* getHead() {
            return seek(head);
        }
        inline int getIntFromHead() { //not check, check outside!
            char tmp[4];
            for ( int i = 0 ; i < 4 ; i++ ) tmp[i] = buffer[(head+i)%BUFSIZE];
            return ntohl(*(int*)tmp);
//            return ntohl(*(int*)seek(head));
        }
        inline void push(int quantity) { //not check, check outside!
            tail = (tail + quantity) % BUFSIZE;
            count += quantity;
        }
        inline void pop(int quantity) { //not check, check outside!
            if (head + quantity < BUFSIZE)
                memset(buffer+head,0,sizeof(quantity));
            else {
                memset(buffer+head,0,sizeof(BUFSIZE-head));
                memset(buffer,0,sizeof(quantity-BUFSIZE+head));
            }
            head = (head + quantity) % BUFSIZE;
            count -= quantity;
        }
        inline void copyFromHead(void* dest, int sz) {
            if (head + sz < BUFSIZE)
                memmove(dest, buffer+head, sz);
            else {
                memmove(dest, buffer+head, BUFSIZE - head);
                memmove((char*)dest+(BUFSIZE-head), buffer, sz-BUFSIZE+head);
            }
        }
        inline void copyIntoTail(const void* src, int sz) {
            if (tail+sz < BUFSIZE)
                memmove(getTail(), src, sz);
            else {
                memmove(getTail(), src, BUFSIZE-tail);
                memmove(buffer, (char*)src+(BUFSIZE-tail), sz-BUFSIZE+tail);
            }
        }
    };

    buffer_t recvbuf;
    buffer_t sendbuf;
    
public:
    int fd;

private:
    void debugPrint(char*,int);
};

}

#endif
