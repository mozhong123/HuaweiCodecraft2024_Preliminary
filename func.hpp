#ifndef __FUNC_H__
#define __FUNC_H__
#include <bits/stdc++.h>
#include "robot.hpp"
#include "berth.hpp"
#include "goods.hpp"
#include "config.hpp"
#include "Astar.h"
using namespace std;


int robot_find(int x, int y)
{
    for (int i = 0; i < robot_num; i++)
    {
        if (robot[i].x == x && robot[i].y == y)
            return i;
    }
    return -1;
}

// int goodsbfs(int rest, int id)
// {
//     queue<pair<int, int>> q;
//     q.push({goods[id].x, goods[id].y});
//     while (!q.empty())
//     {
//         pair<int, int> cur = q.front();
//         q.pop();
//         for (int i = 0; i < 4; i++)
//         {
//             int x = cur.first + px[i], y = cur.second + py[i];
//             int is_robot = robot_find(x, y);
//             if (is_robot != -1)
//             {
//                 // 如果机器人没搬运货物
//                 if (!robot[is_robot].goods)
//                 {
//                     // 如果该机器人搬运该货物的收益最高
//                 }
//             }
//         }
//     }
// }
void check_get(Robot &robot, Goods &good)
{
    if (good.x == robot.x && good.y == robot.y)
    {
        printf("get %d %d\n", rand() % 4); // 修改
    }
}

// void check_pull(Robot& robot,Berth& berth){
//     if(berth.x == robot.x && berth.y == robot.y){
//         printf("pull %d %d\n", rand() % 4); // 修改
//     }
// }

void recover_map()
{ // 初始化 闭集 和 障碍
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (mp[i][j] == '*' || mp[i][j] == '#')
                closeAndBarrierList[i][j] = true;
            else
                closeAndBarrierList[i][j] = false;
        }
    }
    for (int i = 0; i < robot_num; i++)
    { // 机器人也算障碍
        closeAndBarrierList[robot[i].x][robot[i].y] = true;
    }
}

void distributor(Robot &robot, vector<Goods> &goods, vector<Berth> &berths, int zhen)
{
    int mind = -500;
    for (int i = 0; i < goods.size(); i++)
    {
        // 货物已经被选中或者货物消亡则不遍历
        if (goods[i].chosed || goods[i].zhen_id + 1000 < zhen)
            continue;
        // 货物选择标准（可以替换）
        int temp = 0.6 * goods[i].value - 0.4 * (abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y));
        if (temp > mind)
        {
            mind = temp;
            robot.chosed = true;
            robot.target_get = i;
        }
    }
    if (mind == -500)
        return;
    // cout<<robot.robot_id<<" 选中 "<<robot.target_get<<endl;
    mind = 4000;
    // 将货物设置为被选中
    goods[robot.target_get].chosed = true;
    // 为货物挑选最近的港口
    for (int j = 0; j < berth_num; j++)
    {
        int temp = abs(berths[j].x - goods[robot.target_get].x) + abs(berths[j].y - goods[robot.target_get].y);
        if (temp < mind)
        {
            mind = temp;
            robot.target_pull = j;
        }
    }
    // robot.next = greed_next(robot.vis,{robot.x,robot.y} , {berths[robot.target_pull].x , berths[robot.target_pull].y});
    Astar as1;
    recover_map();
    closeAndBarrierList[berths[robot.target_pull].x][berths[robot.target_pull].y] = false;
    auto rs = as1.findway(Point{goods[robot.target_get].x, goods[robot.target_get].y}, Point{berths[robot.target_pull].x, berths[robot.target_pull].y});
    if (rs == nullptr)
    {
        robot.chosed = 0;
        return;
    }
    for (; rs != nullptr; rs = rs->father)
    {
        if (rs->father == nullptr)
            break;
        int tempx = rs->x - rs->father->x, tempy = rs->y - rs->father->y;
        robot.op.push(dir[{tempx, tempy}]);
        // cout<<"berth"<<rs->x<<" "<<rs->y<<endl;
    }
    recover_map();
    // robot.next = greed_next(robot.vis,{robot.x,robot.y} , {goods[robot.target_get].x , goods[robot.target_get].y});
    Astar as2;
    closeAndBarrierList[goods[robot.target_get].x][goods[robot.target_get].y] = false;
    rs = as2.findway(Point{robot.x, robot.y}, Point{goods[robot.target_get].x, goods[robot.target_get].y});
    if (rs == nullptr)
    {
        robot.chosed = 0;
        while (!robot.op.empty())
            robot.op.pop();
        return;
    }
    for (; rs != nullptr; rs = rs->father)
    {
        if (rs->father == nullptr)
            break;
        int tempx = rs->x - rs->father->x, tempy = rs->y - rs->father->y;
        robot.op.push(dir[{tempx, tempy}]);
        // cout<<"good"<<rs->x<<" "<<rs->y<<endl;
    }
}

void redistribute(Robot &robot, vector<Goods> &goods, vector<Berth> &berths, int zhen)
{
    if (robot.goods)
    {
        recover_map();
        Astar as1;
        closeAndBarrierList[berths[robot.target_pull].x][berths[robot.target_pull].y] = false;
        auto rs = as1.findway(Point{robot.x, robot.y}, Point{berths[robot.target_pull].x, berths[robot.target_pull].y});
        if (rs == nullptr)
        {
            robot.chosed = 0;
            return;
        }
        for (; rs != nullptr; rs = rs->father)
        {
            if (rs->father == nullptr)
                break;
            int tempx = rs->x - rs->father->x, tempy = rs->y - rs->father->y;
            robot.op.push(dir[{tempx, tempy}]);
            // cout<<"berth"<<rs->x<<" "<<rs->y<<endl;
        }
        return;
    }
    distributor(robot, goods, berth, zhen);
}

bool berth_compare(pair<int, float>& p1, pair<int, float>& p2) {
    return p1.second < p2.second;
}

bool berth_opposite_compare(pair<int, float>& p1, pair<int, float>& p2) {
    return p1.second > p2.second;
}
int good_find(int x,int y,int cnt_zhen,int zhen)
{
    //返回存在着的货物
    for(int i = 0;i < goods.size();i++)
    {
        if(goods[i].x == x&&goods[i].y == y&&(zhen - goods[i].zhen_id) + cnt_zhen < 1000)
            return i;
    }
    return -1;
}
bool robot2good(Robot &r,int zhen)
{
    queue<pair<int, int>> q;
    q.push({r.x, r.y});
    map<pair<int, int>, pair<int, pair<int,int> >> path;
    path[{r.x, r.y}] = {0, {-1,-1}};
    pair<double,int> find = {-1,-1};//vpt,good_id;
    while (!q.empty())
    {
        pair<int, int> cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int x = cur.first + px[i], y = cur.second + py[i];
            if (x >= 0 && y >= 0 && x < 200 && y < 200 &&
                mp[x][y] != '*' && mp[x][y] != '#' && path.count({x,y}) == 0)
            {
                path[{x,y}] = {path[{cur.first,cur.second}].first + 1,{cur.first,cur.second}};
                int g = good_find(x,y,path[{x,y}].first,zhen);
                if(g != -1)
                {
                    if(goods[g].chosed == false&&!goods[g].op.empty())
                    {
                        double vpt = goods[g].value*1.0/(goods[g].op.size()+path[{x,y}].first);
                        if(vpt > find.first)
                            find = {vpt,g};
                    }
                }
                q.push({x,y});
            }
        }
    }
    if(find.second != -1)//找到了
    {
        r.target_get = find.second;
        r.chosed = 1;//目前先不每帧都bfs
        goods[r.target_get].chosed = true;
        r.op = goods[r.target_get].op;
        find = {goods[find.second].x,goods[find.second].y};
        while(path[find].second.first != -1)
        {
            r.op.push(dir[{find.first - path[find].second.first,find.second - path[find].second.second}]);
            find = path[find].second;
        }
        return true;
    }
    return false;
}
#endif
