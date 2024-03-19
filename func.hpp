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

struct Node {
    int x, y;
};

void bfs(int ind,int startX, int startY) {  //初始化 地图数据
    queue<Node> q;
    q.push({startX, startY});
    dis[ind][startX][startY] = 0;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        int x = current.x;
        int y = current.y;
        int dist = dis[ind][x][y];

        // Check neighbors
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};
        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];
            if (newX >= 0 && newX < map_size && newY >= 0 && newY < map_size && mp[newX][newY] != '*' && mp[newX][newY] != '#' && dist + 1 < dis[ind][newX][newY]) {
                dis[ind][newX][newY] = dist + 1;
                q.push({newX, newY});
            }
        }
    }
}


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
    double mind = -500;
    for(int i = 0 ; i < goods.size() ; i++){
        //货物已经被选中或者货物消亡则不遍历
        if(goods[i].chosed || goods[i].zhen_id + 1000 < zhen) continue;
        //货物选择标准（可以替换）
        // int temp = 0.6*goods[i].value - 0.4*(abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y));
        int pull = pos[goods[i].x][goods[i].y];
        double temp = (5.0*goods[i].value + 2.0*berth[pull].goods_num)/(double)((abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y))+dis[pull][goods[i].x][goods[i].y]);
        // int tmp_pull = -1;
        // for(int j =  0;j < berth_num;j ++)
        // {   
        //     double tmp = (5.0*goods[i].value + 2.0*berth[j].goods_num)/(double)((abs(robot.x - goods[i].x) + abs(robot.y - goods[i].y))+dis[j][goods[i].x][goods[i].y]);
        //     if(tmp > temp)
        //         temp = tmp,tmp_pull = j;
        // }
        if(temp > mind){
            mind = temp;
            robot.chosed = true;
            robot.target_get = i;
            robot.target_pull = pull;
        }
    } 
    if(mind == -500) return;
    //cout<<robot.robot_id<<" 选中 "<<robot.target_get<<endl;
    // mind = 4000;
    //将货物设置为被选中
    goods[robot.target_get].chosed = true;
    //为货物挑选最近的港口
    // for(int j = 0 ; j < berth_num ; j++){
    //     if(dis[j][goods[robot.target_get].x][goods[robot.target_get].y] < mind){ 
    //         mind = dis[j][goods[robot.target_get].x][goods[robot.target_get].y];
    //         robot.target_pull = j;
    //     }
    // }
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
    if(robot.op.size() + zhen > goods[robot.target_get].zhen_id + 1000)
    {
        robot.chosed = false;goods[robot.target_get].chosed = false;
        while(!robot.op.empty())
            robot.op.pop();
        return ;
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

void redistribute(Robot& robot , vector<Goods>& goods ,vector<Berth>& berths,int zhen){
    if(robot.goods){
        recover_map();
        Astar as1;  
        closeAndBarrierList[berths[robot.target_pull].x ][ berths[robot.target_pull].y] = false;
        auto rs = as1.findway(Point{robot.x,robot.y}, Point{berths[robot.target_pull].x , berths[robot.target_pull].y});
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
        return ;
    }
    distributor(robot,goods,berth,zhen);
}

bool berth_compare(pair<int, float>& p1, pair<int, float>& p2) {
    return p1.second < p2.second;
}

bool berth_opposite_compare(pair<int, float>& p1, pair<int, float>& p2) {
    return p1.second > p2.second;
}
#endif