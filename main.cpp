#include <bits/stdc++.h>
#include"robot.hpp"
#include"berth.hpp"
#include"config.hpp"
#include"control.hpp"
#include"goods.hpp"
#include"boat.hpp"
#include"func.hpp"
// #include"func.hpp"
// #include"Astar.h"
using namespace std;


int money, boat_capacity, id;
//int gds[map_size][map_size];  // 官方给的，但似乎没用
vector <Goods> goods;  // 货物

void Init()  // 初始化地图与泊位
{
    for(int i = 0; i < map_size; i ++)
        scanf("%s", mp[i]);
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
        robot[i].robot_id = i;
        scanf("%d%d%d%d", &robot[i].goods, &robot[i].x, &robot[i].y, &robot[i].status);
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

Control control;

int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input(zhen);

        // for(auto good : goods){ //遍历货物// find path 存储好
        //    good.goodbfs();
        // }
        // control 检查
        control.ismove.clear();
        for(int i = 0; i < robot_num; i ++){// 遍历机器人

            distributor(robot[i],goods,berth,zhen); 

            if(robot[i].next == -1) continue; //是否可走

            int next_x = robot[i].x+px[robot[i].next] , next_y  =robot[i].y+py[robot[i].next];

            if(control.check_move(next_x,next_y)){  // 运行阻塞
                printf("move %d %d\n", i , robot[i].next);
                mp[robot[i].x][robot[i].y] = '.';
                mp[next_x][next_y] = 'A';
                if(next_x == goods[robot[i].target_get].x && next_y == goods[robot[i].target_get].y){
                    printf("get %d\n", i );
                    mp[next_x][next_y] = '.';
                }
                // TODO 4 * 4判定
                if(next_x == berth[robot[i].target_pull].x && next_y == berth[robot[i].target_pull].y ){ 
                    printf("pull %d\n", i );
                }
            }   
        }

        for(int i = 0; i < boat_num; i++)
        {
            boat[i].action(zhen);
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}