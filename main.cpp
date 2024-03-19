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
        // 初始化距离
        for(int j=0; j< map_size ;j++)
            for(int k=0; k< map_size ;k++)
                dis[i][j][k] = 400000;
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    for(int i = 0 ; i < map_size ; i++ ){
        for(int j = 0 ; j < map_size ; j++){
            if(mp[i][j] == '*' || mp[i][j] == '#') closeAndBarrierList[i][j] = true;
            else closeAndBarrierList[i][j] = false;
        }
    }
    // 各港口到所有点的距离获取
    for(int i = 0; i < berth_num; i++){
        bfs(i, berth[i].x, berth[i].y);
    }
    //为每个点分配最近的泊点
    for(int i = 0;i < map_size;i++)
    {
        for(int j = 0;j < map_size;j++)
        {
            pos[i][j] = 0;
            for(int k = 1;k < berth_num;k++)
                pos[i][j] = dis[pos[i][j]][i][j] > dis[k][i][j] ? k : pos[i][j];
        }                
    }
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
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input(zhen);

        // for(auto good : goods){ //遍历货物// find path 存储好
        //    good.goodbfs();
        // }
        // control 检查
        control.ismove.clear();
        for(int i = 0; i < robot_num; i ++){// 遍历机器人
            if(!robot[i].status)continue;

            if(!robot[i].chosed)
                distributor(robot[i],goods,berth,zhen); //分配

RE:
            if(robot[i].op.empty())
            {
                if(robot[i].chosed)
                    robot[i].chosed = 0;
                if(robot[i].goods)
                    printf("pull %d\n",i);
                continue;
            }

            int next_x = robot[i].x+px[robot[i].op.top()] , next_y  =robot[i].y+py[robot[i].op.top()];

            if(control.check_move(next_x,next_y)){  // 移动阻塞
                recover_map();
                if(closeAndBarrierList[next_x][next_y]){ //撞墙 重分配
                    if((float)(rand()) / ((float)(RAND_MAX)) < 0.5) continue;
                    while(!robot[i].op.empty())
                        robot[i].op.pop();
                    if(!robot[i].goods)
                        goods[robot[i].target_get].chosed = 0;
                    redistribute(robot[i],goods,berth,zhen);
                    goto RE;
                } 

                printf("move %d %d\n", i , robot[i].op.top());
                robot[i].op.pop();

                //修改属性
                //mp[robot[i].x][robot[i].y] = '.';
                //mp[next_x][next_y] = 'A';
                if(next_x == goods[robot[i].target_get].x && next_y == goods[robot[i].target_get].y&&!robot[i].goods){
                    printf("get %d\n", i );
                    robot[i].goods = true;
                    //robot[i].vis.clear();
                }
                // TODO 4 * 4判定
                if(next_x - berth[robot[i].target_pull].x >= 0 && next_y - berth[robot[i].target_pull].y >=0 &&
                   next_x - berth[robot[i].target_pull].x <= 3 && next_y - berth[robot[i].target_pull].y <=3&&robot[i].goods){ 
                    berth[robot[i].target_pull].goods_num += 1;
                    berth_goods[robot[i].target_pull].push_back(goods[robot[i].target_get].value);
                    berth[robot[i].target_pull].goods_value += goods[robot[i].target_get].value;
                    printf("pull %d\n", i );
                    robot[i].chosed = false;
                    robot[i].goods = false;
                    while(!robot[i].op.empty())
                        robot[i].op.pop();
                    //robot[i].vis.clear();
                }
            }   
        }
        for(int i = 0; i < boat_num; i++)
        {
            // boat[i].action(zhen);
            boat[i].action2(zhen);
        }
        puts("OK");
        fflush(stdout);
    }

    return 0;
}