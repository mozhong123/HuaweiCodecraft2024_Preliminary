#ifndef __BERTH_H__
#define __BERTH_H__
#include"config.hpp"
class Berth
{
    public:
        Berth(double _x,double _y):x(_x),y(_y){}
        double x,y;
        int status;//有没有船/有没有货
        int transport_time;
        int loading_speed;
};

Berth berth[berth_num+1];
#endif