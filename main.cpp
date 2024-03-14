#include <bits/stdc++.h>
#include"robot.hpp"
#include"berth.hpp"
#include"config.hpp"
#include"control.hpp"
#include"goods.hpp"
#include"boat.hpp"
#include"func.hpp"
#include"Astar.h"
using namespace std;


int money, boat_capacity, id;
//int gds[map_size][map_size];  // 官方给的，但似乎没用
vector <Goods> goods;  // 货物
int collect_time[5];  // 存储五艘船装完完当前码头货物的时间
Control control; //检查

void Init()  // 初始化地图与泊位
{
    for(int i = 1; i <= map_size; i ++)
        scanf("%s", mp[i] + 1);
    for(int i = 0; i < berth_num; i ++)
    {
        int id;
        scanf("%d", &id);
        scanf("%d%d%d%d", &berth[id].x, &berth[id].y, &berth[id].transport_time, &berth[id].loading_speed);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}

int Input(int zhen)  // 每帧交互
{
    scanf("%d%d", &id, &money);
    int num;
    scanf("%d", &num);
    for(int i = 1; i <= num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        goods.push_back(Goods(x, y, val, zhen));
    }
    for(int i = 0; i < robot_num; i ++)
    {
        scanf("%d%d%d%d", &robot[i].status, &robot[i].x, &robot[i].y, &robot[i].status);
    }
    for(int i = 0; i < 5; i ++)
    {
        boat[i].id = i;
        scanf("%d%d\n", &boat[i].status, &boat[i].target);
    }
    char okk[100];
    scanf("%s", okk);
    return id;
}

int dx[] = {0,0,-1,1} , dy[] = {1,-1,0,0};

int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input(zhen);
        for(auto good : goods){ //遍历货物
            // find path 存储好
        }
        // control 检查
        control.ismove.clear();
        for(int i = 0; i < robot_num; i ++){// 遍历机器人
            // 根据路径 输出一次
            int tempx = robot[i].x + dx[robot[i].op.front()];
            int tempy = robot[i].y + dy[robot[i].op.front()];
            if(control.check_move(tempx,tempy))
                printf("move %d %d\n", i, rand() % 4); //  0 表示右移一格  1 表示左移一格  2 表示上移一格  3 表示下移一格
                check_get(robot[i],goods[robot[i].target_get]); //输出get
                check_pull(robot[i],berth[robot[i].target_pull]); //输出pull
        }
        for(int i = 0; i < boat_num; i ++)
        {
            //boat[i].judge_full(); 输出船的指令
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
