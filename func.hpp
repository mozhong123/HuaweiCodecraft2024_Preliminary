#ifndef __FUNC_H__
#define __FUNC_H__
#include <bits/stdc++.h>
#include "robot.hpp"
#include "berth.hpp"
#include "goods.hpp"
#include "config.hpp"
#include "Astar.h"
using namespace std;

std::map<pair<int,int> , int> dir = {
    {{0,1},0}, {{0,-1},1},
    {{-1,0},2}, {{1,0},3}
};

// int goodsbfs(int rest,  Goods *good)
// {
//     if (rest == 0) // 无可用机器人则返回-1
//         return -1;
//     queue<pair<int, int>> q;
//     q.push(make_pair(good->x, good->y));
//     bool r = 0, p = 0;
//     bool vis[201][201];
//     memset(vis, 0, sizeof(vis));
//     map<pair<int, int>, pair<int, int>> path;
//     pair<int, int> start, end;
//     path[q.front()] = make_pair(-1, -1);
//     Robot *tmp = nullptr;
//     Berth *tmpb = nullptr;
//     while (!q.empty())
//     {
//         pair<int, int> cur = q.front();
//         q.pop();
//         for (int i = 0; i < 3; i++)
//         {
//             int xx = cur.first + px[i], yy = cur.second + py[i];
//             // 不是障碍物
//             if (mp[xx][yy] != '*' && mp[xx][yy] != '#' && !vis[xx][yy]) //(这里应该改为机器人set中的搜索)
//             {
//                 if (mp[xx][yy] == 'A')
//                 {
//                     if (!r)
//                     { // 如果还没确定机器人
//                         // 如果机器人没有被选择，且不在恢复，且没有在运输则选择
//                         tmp = robot_find(xx, yy);
//                         if (!tmp->chosed && !tmp->goods && tmp->status)
//                         {
//                             r = 1;
//                             // 此处id应该为物品的索引？或者物品指针
//                             tmp->target_get = good;
//                             tmp->chosed = 1;
//                             start = make_pair(xx, yy); // 记录机器人在数组中的位置
//                             // 确定之后该位置就可以视为非障碍物
//                             q.push(make_pair(xx, yy));
//                             // 剩余机器人数量-1
//                             Robot::robot_rest--;
//                         }
//                         else
//                             tmp = nullptr;
//                     }
//                 }
//                 if (mp[xx][yy] == 'B')
//                 {
//                     if (!p)
//                     {
//                         // 未确定泊点则确定泊点
//                         tmpb = berth_find(xx,yy);
//                         p = 1;
//                         end = make_pair(xx, yy);
//                     }
//                     // 泊点可以视为空地
//                     q.push(make_pair(xx, yy));
//                 }
//                 if (mp[xx][yy] == '.')
//                     // 空地
//                     q.push(make_pair(xx, yy));
//                 path[make_pair(xx, yy)] = cur;
//                 vis[xx][yy] = 1;
//                 if (p && r)
//                 {
//                     // 此处id应为berth的id
//                     tmp->target_pull = tmpb;
//                     // 都找到则停止
//                     break;
//                 }
//             }
//         }
//         if (p && r)
//             break;
//     }
//     // 没找到则返回-1
//     if (!p || !r)
//         return -1;
//     // 寻找路径
//     // 取货路径
//     queue<int> op; // 0右，1左，2上，3下，4取，5放
//     pair<int, int> last = path[start];
//     while (last.first != -1)
//     {
//         if (last.first > start.first)
//             op.push(0);
//         else if (last.first < start.first)
//             op.push(1);
//         else if (last.second > start.second)
//             op.push(3);
//         else
//             op.push(2);
//         start = last;
//         last = path[start];
//     }
//     op.push(4);
//     // 放货路径
//     stack<int> reverse_op;
//     last = path[end];
//     reverse_op.push(5);
//     while (last.first != -1)
//     {
//         if (last.first > end.first)
//             reverse_op.push(1);
//         else if (last.first < end.first)
//             reverse_op.push(0);
//         else if (last.second < end.second)
//             op.push(2);
//         else
//             op.push(3);
//         end = last;
//         last = path[end];
//     }
//     while (!reverse_op.empty())
//     {
//         op.push(reverse_op.top());
//         reverse_op.pop();
//     }
//     tmp->op = op;
//     // 找到返回0
//     return 1;
// }
// void check_get(Robot& robot,Goods& good){
//     if(good.x == robot.x && good.y == robot.y){
//         printf("get %d %d\n", rand() % 4); // 修改
//     }
// }

// void check_pull(Robot& robot,Berth& berth){
//     if(berth.x == robot.x && berth.y == robot.y){
//         printf("pull %d %d\n", rand() % 4); // 修改
//     }
// }

void recover_map(){ // 初始化 闭集 和 障碍
    for(int i = 0 ; i < map_size ; i++ ){
        for(int j = 0 ; j < map_size ; j++){
            if(mp[i][j] == '*' || mp[i][j] == '#') closeAndBarrierList[i][j] = true;
            else closeAndBarrierList[i][j] = false;
        }
    }
    for(int i = 0 ; i < robot_num ;i++){ //机器人也算障碍
        closeAndBarrierList[robot[i].x][robot[i].y] = true;
    }
}

void distributor(Robot& robot , vector<Goods>& goods ,vector<Berth>& berths,int zhen){
    int mind = -500;
    for(int i = 0 ; i < goods.size() ; i++){
        //货物已经被选中或者货物消亡则不遍历
        if(goods[i].chosed || goods[i].zhen_id + 1000 < zhen) continue;
        //货物选择标准（可以替换）
        int temp = 0.6*goods[i].value - 0.4*(abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y));
        if(temp > mind){
            mind = temp;
            robot.chosed = true;
            robot.target_get = i;
        }
    } 
    if(mind == -500) return;
    //cout<<robot.robot_id<<" 选中 "<<robot.target_get<<endl;
    mind = 4000;
    //将货物设置为被选中
    goods[robot.target_get].chosed = true;
    //为货物挑选最近的港口
    for(int j = 0 ; j < berth_num ; j++){
        int temp = abs(berths[j].x - goods[robot.target_get].x) + abs(berths[j].y - goods[robot.target_get].y);
        if(temp < mind){ 
            mind = temp;
            robot.target_pull = j;
        }
    }
    //robot.next = greed_next(robot.vis,{robot.x,robot.y} , {berths[robot.target_pull].x , berths[robot.target_pull].y});
    Astar as1;  
    recover_map();
    closeAndBarrierList[berths[robot.target_pull].x ][ berths[robot.target_pull].y] = false;
    auto rs = as1.findway(Point{goods[robot.target_get].x,goods[robot.target_get].y}, Point{berths[robot.target_pull].x , berths[robot.target_pull].y});
    if(rs == nullptr)
    {
        robot.chosed = 0;
        return ;
    }
    for (; rs != nullptr; rs = rs->father) {
        if(rs->father == nullptr) break;
        int tempx = rs->x - rs->father->x , tempy = rs->y - rs->father->y;
        robot.op.push(dir[{tempx,tempy}]);
        //cout<<"berth"<<rs->x<<" "<<rs->y<<endl;
    }
    recover_map();
    //robot.next = greed_next(robot.vis,{robot.x,robot.y} , {goods[robot.target_get].x , goods[robot.target_get].y});
    Astar as2;
    closeAndBarrierList[goods[robot.target_get].x][goods[robot.target_get].y] = false;
    rs = as2.findway(Point{robot.x,robot.y}, Point{goods[robot.target_get].x , goods[robot.target_get].y});
    if(rs == nullptr)
    {
        robot.chosed = 0;
        while(!robot.op.empty())
            robot.op.pop();
        return;
    }
    for (; rs != nullptr; rs = rs->father) {
        if(rs->father == nullptr) break;
        int tempx = rs->x - rs->father->x , tempy = rs->y - rs->father->y;
        robot.op.push(dir[{tempx,tempy}]);
        //cout<<"good"<<rs->x<<" "<<rs->y<<endl;
    }
}

bool berth_compare(pair<int, int>& p1, pair<int, int>& p2) {
    return p1.second < p2.second;
}

bool berth_opposite_compare(pair<int, int>& p1, pair<int, int>& p2) {
    return p1.second > p2.second;
}
#endif