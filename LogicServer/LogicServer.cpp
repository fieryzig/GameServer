/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 11:38:47 AM PST
 * File Name : LogicServer.cpp
 */


#include "LogicServer.h"
#include "rapidjson/document.h"
#include "utils/FileUtils.h"

#include <iostream>

using namespace std;

namespace fieryzig {

void LogicServer::connectGateway()
{
    gatefd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in gatewayAddr;

    gatewayAddr.sin_family = AF_INET;
    gatewayAddr.sin_port = 8898;
    inet_aton("127.0.0.1",&(gatewayAddr.sin_addr));
    
    bind(gatefd, (struct sockaddr*)&gatewayAddr, sizeof(gatewayAddr));
    
    gatewayAddr.sin_family = AF_INET;
    gatewayAddr.sin_port = htons(gateway.second);
    inet_aton(gateway.first.c_str(), &(gatewayAddr.sin_addr));
    ::connect(gatefd, (struct sockaddr*)&gatewayAddr, sizeof(gatewayAddr));

    Client *gate = new Client(gatefd);
    ev.data.ptr = gate;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, gatefd, &ev);
}

void LogicServer::Run()
{
    int n = epoll_wait(epfd, events, 20, 500);
    for (int i = 0; i < n; i++) {
        if (events[i].data.fd == listenfd) {
            acceptConnect();
        }
        else {
            Client* cli = (Client*)events[i].data.ptr;
            int ret = cli->Recv();
            PacketInfo pi; string content;
            bool has = cli->hasMsg(&pi, content);
            if (has) {
                Dispatch(cli, &pi, content);
            }
        }
    }

}

void LogicServer::Dispatch(Client* cli, PacketInfo *ppi, string content)
{
    cout << "logic recv: " << content << endl;
    switch(ppi->srcType) {
        case ServerType::CLIENT:
        {
            // impossible
            break;
        }
        case ServerType::GATEWAY:
        {
            if (content == "Login") {
                fd_map[ppi->uid] = ppi->clifd;
                Player* player = new Player(ppi->uid);
                pl_map[ppi->uid] = player;
                cout << "User " << ppi->uid << "Login" << endl;
            }
            else if (content == "Logout") {
                delete pl_map[ppi->uid];
                pl_map.erase(ppi->uid);
                fd_map.erase(ppi->uid);
                cout << "User " << ppi->uid << "Logout" << endl;
            }
            // Other Msg
            break;
        }
        case ServerType::LOGIC:
        {
            break;
        }
        // DB Cache
    }

}

RS LogicServer::SetConfig(const char* configFile)
{
    string json = GetStringFromFile(configFile);
    if (json == "") {
        cout << configFile << " doesn't exist!" << endl;
        return RS::FAILED;
    }
    using rapidjson::Document;
    
    Document d;
    d.Parse(json.c_str());
    
    string type; ServerType st;
    int port = -1;
    string ip;
    
    using rapidjson::Value;
    
    Value& v = d["ServerType"];
    if (v.IsString()) {
        type = v.GetString();
        if (type == "CLIENT") {
            st = ServerType::CLIENT;
        }
        else if (type == "GATEWAY") {
            st = ServerType::GATEWAY;
        }
        else if (type == "LOGIC") {
            st = ServerType::LOGIC;
        }
        else {
            cout << "Wrong Server Type!" << endl;
            return RS::FAILED;
        }
    }
    v = d["IP"];
    if (v.IsString()) {
        ip = v.GetString();
    }
    else {
        cout << "Wrong IP Format!" << endl;
    }
    
    v = d["PORT"];
    if (v.IsInt()) {
        port = v.GetInt();
    }
    else {
        cout << "Wrong port!" << endl;
    }
    
    v = d["GATEWAY"];
    if (v.IsArray()) {
        for (int i = 0; i < v.Size(); i++) {
            Value& content = v[i];
            string ip; int port;
            if (content.HasMember("IP") && content["IP"].IsString()) {
                ip = content["IP"].GetString();
            }
            if (content.HasMember("PORT") && content["PORT"].IsInt()) {
                port = content["PORT"].GetInt();
            }
            gateway = make_pair(ip,port);
        }
    }

    config = new ServerConfig(st, ip, port);
    return RS::SUCCESS;
}

void LogicServer::acceptConnect()
{
    socklen_t clilen;
    connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
    Client *client = new Client(connfd);

    // DB Cache Server ? may connect in?
    //
    ev.data.ptr = client;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
}

}
