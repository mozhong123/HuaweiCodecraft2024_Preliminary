#include <bits/stdc++.h>
#include <math.h>
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
        for (int i = 0; i < robot_num; i++)
        {
            if (robot[i].chosed && robot[i].op.empty())
                robot[i].chosed = 0, robot[i].target_get = -1;
            if (!robot[i].chosed && robot[i].status)
            {
                // 如果机器人空闲则bfs遍历最好的货物
                robot2good(robot[i], zhen);
            }
            if (robot[i].status == 0)
            {
                if (robot[i].goods)
                    printf("pull %d\n", i);
                else
                {
                    if (robot[i].target_get != -1)
                        goods[robot[i].target_get].chosed = 0;   
                }
                robot[i].chosed = 0,robot[i].target_get = -1;
                while(!robot[i].op.empty())
                        robot[i].op.pop();
                robot2good(robot[i],zhen);
            }
        }
        // 死锁控制
        bool vis[10];
        memset(vis, 0, sizeof(vis));
        for (int i = 0; i < robot_num; i++)
        {
            // 如果当前机器人陷入之前机器人的遍历，则跳过
            if (vis[i])
                continue;
            vis[i] = 1; // 标记当前机器人为遍历过
            if (!robot[i].status || robot[i].op.empty())
                continue;
            int next_x = robot[i].x + px[robot[i].op.top()], next_y = robot[i].y + py[robot[i].op.top()];
            int end = robot_find(next_x, next_y);
            while (end != -1 && !vis[end])
            {
                if (!robot[end].status)
                    break;
                if (robot[end].op.empty())
                    break;
                vis[end] = 1;
                next_x = robot[end].x + px[robot[end].op.top()];
                next_y = robot[end].y + py[robot[end].op.top()];
                end = robot_find(next_x, next_y);
            }
            if (end != -1)
            {
                // 1.status = 0，先不解决
                // 2. 如果遍历了之前遍历过的，且不是环的起点则不用操作。
                if (end != i && vis[end])
                    continue;
                vis[end] = 1;
                // 2.end == i，环，让其中任意一个移动到一个空地上
                if (robot[end].op.empty())
                {
                    for (int j = 0; j < 4; j++)
                    {
                        int x = robot[end].x + px[j],
                            y = robot[end].y + py[j];
                        if (robot_find(x, y) == -1 && x < 200 && y < 200 && x >= 0 && y >= 0 && mp[x][y] != '*' && mp[x][y] != '#')
                        {
                            robot[end].op.push(dir[{-px[j], -py[j]}]);
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
                            if (robot_find(x, y) == -1 && x < 200 && y < 200 && x >= 0 && y >= 0 && mp[x][y] != '*' && mp[x][y] != '#')
                            {
                                flag = 1;
                                robot[end].op.push(dir[{-px[j], -py[j]}]);
                                robot[end].op.push(dir[{px[j], py[j]}]);
                                break;
                            }
                        }
                        if (!flag && !robot[end].op.empty())
                        {
                            end = robot_find(robot[end].x + px[robot[end].op.top()],
                                             robot[end].y + py[robot[end].op.top()]);
                            if (end == i) // 每个机器人都找不到出路
                                break;
                        }
                        else
                            break;
                    }
                }
            }
        }
        map<pair<int,int>,bool > track;
        for (int i = 0; i < robot_num; i++)
        {
            if (robot[i].status && robot[i].chosed && !robot[i].op.empty()) // 正常状态
            {
                int nx = robot[i].x + px[robot[i].op.top()],
                    ny = robot[i].y + py[robot[i].op.top()];
                if(track[{nx,ny}] == false)
                    printf("move %d %d\n", i, robot[i].op.top());
                else continue;
                track[{nx,ny}] = 1;
                robot[i].op.pop();

                // 不考虑阻塞碰撞等情况
                if (mp[nx][ny] == 'B' && robot[i].goods)
                {
                    berth[goods[robot[i].target_get].target_pull].goods_num += 1;
                    printf("pull %d\n", i);
                    robot[i].chosed = false;
                }
                else if (nx == goods[robot[i].target_get].x && ny == goods[robot[i].target_get].y && !robot[i].goods)
                    printf("get %d\n", i);
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