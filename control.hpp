#ifndef __CONTROL_H__
#define __CONTROL__
// 检查每一帧机器人移动
#include"config.hpp"
#include<map>
using namespace std;

struct Control{
    map<pair<int,int>,bool> ismove;
    bool check_move(int x ,int y){
        if (ismove[{x,y}]) return false;
        else {
            ismove[{x,y}] = true;
            return true;
        }
    }
};

#endif