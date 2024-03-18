#include <bits/stdc++.h>
#include "robot.hpp"
#include "berth.hpp"
#include "config.hpp"
#include "control.hpp"
#include "goods.hpp"
#include "boat.hpp"
#include "func.hpp"
// #include"func.hpp"
// #include"Astar.h"
using namespace std;

int money, boat_capacity, id;
// int gds[map_size][map_size];  // 官方给的，但似乎没用
vector<Goods> goods; // 货物

void Init() // 初始化地图与泊位
{
    for (int i = 0; i < map_size; i++)
        scanf("%s", mp[i]);
    for (int i = 0; i < berth_num; i++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
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
    printf("OK\n");
    fflush(stdout);
}

int Input(int zhen) // 每帧交互
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for (int i = 1; i <= num; i++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        goods.push_back(Goods(x, y, val, zhen));
    }
    for (int i = 0; i < robot_num; i++)
    {
        robot[i].robot_id = i;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
    }
    for (int i = 0; i < 5; i++)
    {
        boat[i].id = i;
        boat[i].capacity = boat_capacity;
        scanf("%d%d\n", &boat[i].status, &boat[i].target);
    }
    char okk[100];
    scanf("%s", okk);
    return id;
}

Control control;

int main()
{
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++)
    {
        int id = Input(zhen);

        // for(auto good : goods){ //遍历货物// find path 存储好
        //    good.goodbfs();
        // }
        // 死锁防止
        for (int i = 0; i < robot_num; i++)
        {
            if (!robot[i].status || robot[i].op.empty())
                continue;
            int next_x = robot[i].x + px[robot[i].op.top()], next_y = robot[i].y + py[robot[i].op.top()];
            int end = robot_find(next_x, next_y);
            while (end != -1)
            {
                if (end == i)
                    break;
                if (!robot[end].status)
                    break;
                if (robot[end].op.empty())
                    break;
                next_x = robot[end].x + px[robot[end].op.top()];
                next_y = robot[end].y + py[robot[end].op.top()];
                end = robot_find(next_x, next_y);
            }
            if (end != -1)
            {
                // 1.status = 0，先不解决
                    // 2.end == i，环，让其中任意一个移动到一个空地上
                    if (robot[end].op.empty())
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            int x = robot[end].x + px[j],
                                y = robot[end].y + py[j];
                            if (robot_find(x, y) == -1 && mp[x][y] != '*' && mp[x][y] != '#')
                            {
                                robot[end].op.push(dir[{-px[j], -py[y]}]);
                                robot[end].op.push(dir[{px[j], py[j]}]);
                                break;
                            }
                        }
                    }
                    else
                    {
                        bool flag = 0;
                        while (!flag)
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                int x = robot[end].x + px[j],
                                    y = robot[end].y + py[j];
                                if (robot_find(x, y) == -1 && mp[x][y] != '*' && mp[x][y] != '#')
                                {
                                    flag = 1;
                                    robot[end].op.push(dir[{-px[j], -py[y]}]);
                                    robot[end].op.push(dir[{px[j], py[j]}]);
                                    break;
                                }
                            }
                            if (!flag && !robot[end].op.empty())
                            {
                                end = robot_find(robot[end].x + px[robot[end].op.top()],
                                                robot[end].y + py[robot[end].op.top()]);
                                if(end == i)//每个机器人都找不到出路
                                    break;
                            }
                            else
                                break;
                        }
                    }
            }
        }
        // control 检查
        control.ismove.clear();
        for (int i = 0; i < robot_num; i++)
        { // 遍历机器人
            if (!robot[i].status)
                continue;

            if (!robot[i].chosed)
                distributor(robot[i], goods, berth, zhen); // 分配

RE:
            if(robot[i].op.empty())
            {
                if (robot[i].chosed)
                    robot[i].chosed = 0;
                if (robot[i].goods)
                    printf("pull %d\n", i);
                continue;
            }

            int next_x = robot[i].x + px[robot[i].op.top()], next_y = robot[i].y + py[robot[i].op.top()];

            if(control.check_move(next_x,next_y)){  // 移动阻塞
                recover_map();
                if(closeAndBarrierList[next_x][next_y]){ //撞墙 重分配
                    while(!robot[i].op.empty())
                        robot[i].op.pop();
                    goods[robot[i].target_get].chosed = 0;
                    redistribute(robot[i],goods,berth,zhen);
                    goto RE;
                } 

                printf("move %d %d\n", i , robot[i].op.top());
                robot[i].op.pop();

                // 修改属性
                // mp[robot[i].x][robot[i].y] = '.';
                // mp[next_x][next_y] = 'A';
                if (next_x == goods[robot[i].target_get].x && next_y == goods[robot[i].target_get].y && !robot[i].goods)
                {
                    printf("get %d\n", i);
                    robot[i].goods = true;
                    // robot[i].vis.clear();
                }
                // TODO 4 * 4判定
                if (next_x - berth[robot[i].target_pull].x >= 0 && next_y - berth[robot[i].target_pull].y >= 0 &&
                    next_x - berth[robot[i].target_pull].x <= 3 && next_y - berth[robot[i].target_pull].y <= 3 && robot[i].goods)
                {
                    berth[robot[i].target_pull].goods_num += 1;
                    printf("pull %d\n", i);
                    robot[i].chosed = false;
                    while (!robot[i].op.empty())
                        robot[i].op.pop();
                    // robot[i].vis.clear();
                }
            }
        }
        for (int i = 0; i < boat_num; i++)
        {
            boat[i].action(zhen);
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}