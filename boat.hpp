#ifndef __BOAT_H__
#define __BOAT_H__
#include"config.hpp"
class Boat
{
    public:
    Boat(double);
    double capacity;
    int status;//0：运输中;1正常运行状态;2：泊位外等待状态
    int target;//目标泊位
    void Chose_Berth();
};

Boat boat[boat_num+5];
Boat::Boat(double _cap):capacity(_cap){}
#endif