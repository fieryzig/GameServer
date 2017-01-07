/*
 * Author : fieryzig
 * Create Time : Thu 05 Jan 2017 11:46:52 PM PST
 * File Name : GateServer.h
 */

#ifndef GATESERVER
#define GATESERVER

#include "../Common/BaseServer/BaseServer.h"

#include <vector>
#include <set>
#include <algorithm>

using namespace std;

namespace fieryzig{

class GateServer : public BaseServer
{
public:
    void Run();
    void Dispatch(Client*,PacketInfo*,string);
    void acceptConnect();
    RS SetConfig(const char*);

private:
    int Login()
    {
        return time(NULL) & 0xffffffff;
    }

    vector<Client*> Logics;
    set<Client*> Clients;
    vector< pair<string,int> > logic_ip;
};

}
#endif
