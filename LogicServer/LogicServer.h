/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 11:34:31 AM PST
 * File Name : LogicServer.h
 */

#ifndef LOGICSERVER_H
#define LOGICSERVER_H

#include "../Common/BaseServer/BaseServer.h"
#include "Player.h"

#include <vector>
#include <set>
#include <map>

using namespace std;

namespace fieryzig {
    
class LogicServer : public BaseServer
{
public:
    void Run();
    void Dispatch(Client*,PacketInfo*,string);
    void acceptConnect();
    RS SetConfig(const char*);
    void connectGateway();
private:

    set<Client*> Clients;
    int gatefd;
    pair<string,int> gateway;
    map<unsigned int,Player*> pl_map;
    map<int,int> fd_map;
};

}

#endif
