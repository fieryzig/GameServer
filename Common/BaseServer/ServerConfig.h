#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <string>
using namespace std;

namespace fieryzig{

class ServerConfig{
public:
    ServerConfig(){}
    ServerConfig(ServerType type, string ip, int port) :
        type(type),ip(ip),port(port) {}
    ~ServerConfig();

    ServerType type;
    string ip;
    int port;
};

}
#endif
