#ifndef __BERTH_H__
#define __BERTH_H__
#include "config.hpp"
class Berth
{
public:
    Berth() {}
    Berth(int _x, int _y, int transport_time, int loading_speed) : x(_x), y(_y)
    {
        this->transport_time = transport_time;
        this->loading_speed = loading_speed;
    }
    int x, y;
    int status; // 有没有船/有没有货
    int transport_time;
    int loading_speed;
};

Berth berth[berth_num + 10];


#endif