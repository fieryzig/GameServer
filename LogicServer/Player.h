/*
 * Author : fieryzig
 * Create Time : Fri 06 Jan 2017 12:26:19 PM PST
 * File Name : Player.h
 */

#ifndef PLAYER_H
#define PLAYER_H

namespace fieryzig{
class Player
{
public:
    Player(int uid):uid(uid){}
    ~Player() = default;

    int uid;
};
}
#endif
