#ifndef __ROBOT_H__
#define __ROBOT_H__
#include"config.hpp"
class Robot
{
public:
    Robot() {}
    Robot(double _x, double _y) : x(_x), y(_y) {}
    double x, y;
    int status;      // 0 : 找货物  1 ：找泊位
    int normal;      // 0/1 正常运行(恢复状态)
    int target_get;  // 目标货物
    int target_pull; // 目标港口
    int carry;  // 0/1 是否携带货物
    void action(int);
    void check();
    void find_path();
};



Robot robot[robot_num+10];
void Robot::action(int op)
{
    if(op == 0)this->x += 1;
    else if(op == 1)this->x -= 1;
    else if(op == 2)this->y -= 1;
    else this->y += 1;
}
#endif