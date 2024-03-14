#include <bits/stdc++.h>
#include"robot.hpp"
#include"berth.hpp"
#include"config.hpp"
#include"goods.hpp"
#include"boat.hpp"
#include"func.hpp"
using namespace std;

const int n = 200;
const int robot_num = 10;
const int berth_num = 10;
const int N = 210;

int money, boat_capacity, id;
char ch[N][N];  // 地图
int gds[N][N];  // 官方给的，但似乎没用
vector <Goods> goods;  // 货物
int collect_time[5];  // 存储五艘船装完完当前码头货物的时间
void Init()  // 初始化地图与泊位
{
    for(int i = 1; i <= n; i ++)
        scanf("%s", ch[i] + 1);
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
        scanf("%d%d%d%d", &robot[i].carry, &robot[i].x, &robot[i].y, &robot[i].normal);
    }
    for(int i = 0; i < 5; i ++)
<<<<<<< HEAD
        scanf("%d%d\n", &boat[i].status, &boat[i].target);
=======
    {
        boat[i].id = i;
        scanf("%d%d\n", &boat[i].status, &boat[i].target);
    }
>>>>>>> d660657df494b8b3c69923159cad33013fef2a5d
    char okk[100];
    scanf("%s", okk);
    return id;
}

int main()
{
    Init();
    for(int zhen = 1; zhen <= 15000; zhen ++)
    {
        int id = Input(zhen);
        for(int i = 0; i < robot_num; i ++)
            printf("move %d %d\n", i, rand() % 4);
        puts("OK");
        fflush(stdout);
    }

    return 0;
}
