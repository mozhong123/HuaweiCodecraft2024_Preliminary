#ifndef __BERTH_H__
#define __BERTH_H__
#include "config.hpp"
#include<vector>
class Berth
{
public:
    Berth() {}
    Berth(int _x, int _y, int transport_time, int loading_speed) : x(_x), y(_y)
    {
        this->transport_time = transport_time;
        this->loading_speed = loading_speed;
    }
    std::vector<int> boat_q; // 港口目前的船只情况
    int x, y;
    int status = 0; // 有没有船，0无1有
    int goods_num = 0; // 现有货物数量
    int goods_value = 0;
    int transport_time;
    int loading_speed;
    int expected = 0; //期望当前港口的货物新增多少
};

std::vector<Berth> berth(berth_num);

#endif