/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 05:33:20 AM PST
 * File Name : GateServer.cpp
 */

#include "GateServer.h"
#include "rapidjson/document.h"

#include "utils/FileUtils.h"

#include <iostream>
#include <algorithm>

using namespace std;

namespace fieryzig{

void GateServer::Run()
{
    int n = epoll_wait(epfd, events, 20, 500);
	for (int i = 0; i < n; i++) {
	    if (events[i].data.fd == listenfd) {
		    acceptConnect();
	    }
	    else {
		    int ret = ((Client*)events[i].data.ptr)->Recv();
	    }
	}

	set<Client*>::iterator it;
	for (it = Clients.begin(); it != Clients.end(); it++) {
	    PacketInfo pi; string content;
	    bool has = (*it)->hasMsg(&pi, content);
	    if (has) {
		    Dispatch((*it), &pi, content);
	    }
	}

	for (it = Clients.begin(); it != Clients.end(); it++) {
	    (*it)->SendRemain();
	}
}

void GateServer::Dispatch(Client* cli, PacketInfo *ppi, string content) {
    cout << "gate recv: " << content << endl;
	switch(ppi->srcType) {
		case ServerType::CLIENT:
        {
		    PacketInfo pi;
		    if (ppi->uid == -1 && content == "Login") {

		        int uid = Login();
		        pi.srcType = config->type;
		        pi.time = time(NULL);
		        pi.uid = uid;
		        pi.svrid = ppi->svrid;
		        pi.clifd = cli->fd;
		        pi.contentLen = 5;
		        
                cli->Send(pi,"Login");
                Logics[pi.svrid]->Send(pi,"Login");
		    }
		    else {
                cout << "DEBUG: send to logic: " << content << endl;
                Logics[ppi->svrid]->Send(*ppi,content);
                if (content == "Logout") {
                    Clients.erase(cli);
                    close(cli->fd);
                }
		    }
		    break;
        }
		case ServerType::GATEWAY:
        {
		    break;
        }
		case ServerType::LOGIC:
        {
		    break;
        }
	}
}

RS GateServer::SetConfig(const char* configFile)
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
        if ( type == "CLIENT" ) {
            st = ServerType::CLIENT;
        }
        else if ( type == "GATEWAY" ) {
            st = ServerType::GATEWAY;
        }
        else if ( type == "LOGIC" ) {
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
        /*
        if ( [ip](){ 
                //TODO: Check ip fomat
                return true;
            } == false) {
            cout << "Wrong IP Format!" << endl;
        }
        */
    }
    else {
        cout << "Wrong IP Format!" << endl;
    }

    v = d["PORT"];
    if (v.IsInt()) {
        port = v.GetInt();
    }
    else {
        cout << "Wrong port !" << endl;
    }

    v = d["LOGIC"];
    if (v.IsArray()) {
        for (int i = 0; i < v.Size(); i++) {
            Value& content = v[i];
            if (content.HasMember("IP") && content["IP"].IsString()) {
                //TODO: Check ip format
                string ip = content["IP"].GetString();
                int port = content["PORT"].GetInt();
                logic_ip.push_back(make_pair(ip,port));
            }
        }
    }
    else {
        cout << "Wrong LOGIC server format" << endl;

    }

    config = new ServerConfig(st, ip, port);        
    return RS::SUCCESS;
}

void GateServer::acceptConnect()
{
    socklen_t clilen;
    connfd = accept(listenfd, (sockaddr*)&clientaddr, &clilen);
    Client *client = new Client(connfd);
    string ip = inet_ntoa(clientaddr.sin_addr);
    int port = clientaddr.sin_port;
    if (find(logic_ip.begin(), logic_ip.end(), make_pair(ip,port)) == logic_ip.end()) {
        Clients.insert(client);
        cout << "A Client has connected!" << endl;
    }
    else {
        cout << "Logic connected,";
        Logics.push_back(client);
        cout << "now Logic servers " << Logics.size() << endl; 
    }
    ev.data.ptr = client;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
}


}
