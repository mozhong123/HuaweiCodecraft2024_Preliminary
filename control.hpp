#ifndef __CONTROL_H__
#define __CONTROL__
// 检查每一帧机器人移动
#include"config.hpp"
#include"robot.hpp"


struct Control{
    map<pair<int,int>,bool> ismove;
    bool check_move(pair<int,int>& target){
        if (ismove[target]) return false;
        else {
            ismove[target] = true;
            return true;
        }
    }
};

#endif