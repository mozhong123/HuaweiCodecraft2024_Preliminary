#ifndef __ROBOT_H__
#define __ROBOT_H__
#include"config.hpp"
class Robot
{
    public:
    Robot(){}
    Robot(int _x,int _y):x(_x),y(_y){}
    int x,y;
    int goods;//0 : 未携带物品  1 ：携带物品
    int status;//0：恢复状态 1 ： 正常运行状态
    int target_get;//目标货物
    int target_pull;//目标港口
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