/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 12:20:39 PM PST
 * File Name : main.cpp
 */

#include "LogicServer.h"

using namespace fieryzig;

int main()
{
    LogicServer *logicServer = new LogicServer();
    
    logicServer->SetConfig("logicConfig.json");
    logicServer->InitServer();
    
    logicServer->connectGateway();
    
    while(1)
    {
        logicServer->Run();
    }
    
    return 0;
}

