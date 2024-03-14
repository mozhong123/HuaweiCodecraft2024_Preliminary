#ifndef __ROBOT_H__
#define __ROBOT_H__
#include "config.hpp"
#include "goods.hpp"
#include "berth.hpp"
class Robot
{
public:
    Robot() {}
    Robot(int _x, int _y) : x(_x), y(_y) {}
    int x, y,robot_id;
    int goods;          // 0 : 未携带物品  1 ：携带物品
    int status;         // 0：恢复状态 1 ： 正常运行状态
    Goods *target_get;  // 目标货物
    Berth *target_pull; // 目标港口
    bool chosed;
    queue<int> op; // 指令队列
    void action();
    void check();
    void find_path();
    static int robot_rest;
};

int Robot::robot_rest = 10;

Robot robot[robot_num + 10];
void Robot::action()
{
    if(!op.empty())
    {
        int o = op.front();op.pop();
        printf("move %d %d\n",id,o);
    }
    //是否处于特殊状态
    if(!op.empty())
    {
        if(op.front()==4)
        {
            printf("get %d\n",id);
            op.pop();
        }
        else if(op.front()==5)
        {
            printf("pull %d\n",id);
            op.pop();
        }
    }
}
#endif