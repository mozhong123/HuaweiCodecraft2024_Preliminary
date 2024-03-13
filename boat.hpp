#ifndef __BOAT_H__
#define __BOAT_H__
#include "config.hpp"
class Boat
{
public:
    Boat(int, int);
    int id;  // 船的id
    int capacity; // 船的容量
    int status; // 0：运输中;1：正常运行状态;2：泊位外等待状态
    int target; // 目标泊位
    int exist_goods_num = 0; // 船上的货物数
    int leave_time = 0; // 离开的时间，即货物装满的时间
    void to_berth(int _target);  // 送去泊位
    void sold();  // 卖货
    int judge_full(int zhen, int goods_num,int loading_speed); // 判断是否装满(当前码头货物能装满船则记录船离开时间并返回0；反之则返回传输完当前所有货物时的帧)
};

Boat boat[boat_num + 5];
Boat::Boat(int _cap,int _pos) : capacity(_cap),target(_pos) {}

void Boat::to_berth(int _target)
{
    printf("ship %d %d\n", this->id, _target);
}

void Boat::sold()
{
    printf("go %d\n", this->id);
}

int Boat::judge_full(int zhen, int goods_num, int loading_speed)
{
    int require_good = this -> capacity - this -> exist_goods_num; // 装满所需货物
    if(require_good <= goods_num) // 能装满
    {
        this -> leave_time = zhen + require_good / loading_speed;
        return 0;
    }
    else // 装不满
    {
        this -> exist_goods_num += goods_num;  
        return zhen + ceil(goods_num / loading_speed);
    }
}














#endif