#ifndef __ROBOT_H__
#define __ROBOT_H__
#include"config.hpp"
class Robot
{
    public:
    Robot(){}
    Robot(double _x,double _y):x(_x),y(_y){}
    double x,y;
    int status;//0 : 找货物  1 ：找泊位
    int normal;//0/1正常运行
    int target_get;//目标货物
    int target_pull;//目标港口
    void action();
    void check();
    void find_path();
};


Robot robot[robot_num+10];
#endif