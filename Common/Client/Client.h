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
        char buffer[BUFSIZE];
        int head, tail, count;
        inline int remain() {
            return BUFSIZE - count;
        }
        inline char* seek(int pos) {
            assert(pos<BUFSIZE);
            return (char*)(buffer+pos);
        }
        inline char* getTail() {
            return seek(tail);
        }
        inline char* getHead() {
            return seek(head);
        }
        inline int getIntFromHead() { //not check, check outside!
            return ntohl(*(int*)seek(head));
        }
        inline void push(int quantity) { //not check, check outside!
            tail = (tail + quantity) % BUFSIZE;
            count += quantity;
        }
        inline void pop(int quantity) { //not check, check outside!
            head = (head + quantity) % BUFSIZE;
            count -= quantity;
        }
        inline void copyFromHead(void* dest, int sz) {
            memcpy(dest, buffer+head, sz);
        }
        inline void copyIntoTail(const void* src, int sz) {
            memcpy(getTail(), src, sz);
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
