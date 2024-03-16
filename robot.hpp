#ifndef __ROBOT_H__
#define __ROBOT_H__
#include "config.hpp"
#include "berth.hpp"
#include"Astar.h"
#include<queue>
class Robot
{
public:
    Robot() {}
    Robot(int _x, int _y) : x(_x), y(_y) {}
    int x, y, robot_id;
    int goods;       // 0 : 未携带物品  1 ：携带物品
    int status;      // 0：恢复状态 1 ： 正常运行状态
    int target_get = -1;  // 目标货物
    int target_pull = -1; // 目标港口
    bool chosed = false;
    //std::queue<int> op; // 指令队列
    std::map<std::pair<int,int>,bool> vis;
    std::stack<int> op;
    //int next;
    void action();
    // void check();
    // void find_path();
    static int robot_rest;
};

int Robot::robot_rest = 10;

Robot robot[robot_num + 10];
// void Robot::action()
// {
//     // if (!op.empty())
//     // {
//     //     int o = op.front();
//     //     op.pop();
//     //     printf("move %d %d\n", robot_id, o);
//     // }
//     // // 是否处于特殊状态
//     // if (!op.empty())
//     // {
//     //     if (op.front() == 4)
//     //     {
//     //         printf("get %d\n", robot_id);
//     //         op.pop();
//     //     }
//     //     else if (op.front() == 5)
//     //     {
//     //         printf("pull %d\n", robot_id);
//     //         op.pop();
//     //     }
//     // }

//     //choose()
//     greed()
// }
#endif