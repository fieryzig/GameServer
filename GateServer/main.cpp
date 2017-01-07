/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 06:56:56 AM PST
 * File Name : main.cpp
 */

#include "GateServer.h"

using namespace fieryzig;

int main()
{
    GateServer *gateServer = new GateServer();
    gateServer->SetConfig("gateConfig.json");
    gateServer->InitServer();
    while(1)
    {
        gateServer->Run();
    }
    return 0;
}
