#ifndef __ASTAR_H__
#define __ASTAR_H__
#pragma GCC optimize(3)
#include <iostream>
#include <vector>
#include"config.hpp"
#include <queue>


struct Point {
    int x;
    int y;
    bool operator == (const Point&otherPoint) {
        return x == otherPoint.x && y == otherPoint.y;
    }
};

struct OpenPoint : public Point {
    int cost;                 // 从起点的耗费值
    int pred;                 // 至终点的预测值
    OpenPoint* father;        // 父节点
    OpenPoint() = default;
    OpenPoint(const Point & p, const Point& end, int c, OpenPoint* fatherp): Point(p), cost(c), father(fatherp) {
        //相对位移x,y取绝对值
        int relativex = std::abs(end.x - p.x);
        int relativey = std::abs(end.y - p.y);
        //预测值pred
        pred = relativex + relativey + c;
    }
};


//比较器，用以优先队列的指针类型比较
struct OpenPointPtrCompare {
    bool operator()(OpenPoint* a, OpenPoint* b){
        return a->pred > b->pred;
    }
};
//使用最大优先队列
std::priority_queue<OpenPoint*, std::vector<OpenPoint*>, OpenPointPtrCompare> openlist;
//存储OpenPoint的内存空间
std::vector<OpenPoint> pointList = std::vector<OpenPoint>(map_size*map_size);
//地图数据
//char mapBuffer[map_size][map_size];
//是否可以经过
bool closeAndBarrierList[map_size][map_size];
//深度
//int deepth;

//检查函数 返还成功与否值
inline bool inBarrierAndCloseList(const Point & pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= map_size || pos.y >= map_size)
        return true;
    return closeAndBarrierList[pos.x][pos.y];
}

//创建一个开启点
inline OpenPoint* createOpenPoint(const Point & p, const Point& end, int c, OpenPoint* fatherp) {
    pointList.emplace_back(p, end, c, fatherp);
    return &pointList.back();
}

//四方的位置
Point direction[4] ={ Point{0,1},Point{0,-1},Point{-1,0}, Point{1,0} };

// 开启检查，检查父节点
void open(OpenPoint& pointToOpen,const Point &start, const Point & end) {
    //每检查一次，深度+1
    //deepth++;
    //将父节点从openlist移除
    openlist.pop();
    Point toCreate;
    //检查p点四方的点
    for (int i = 0; i < 4; ++i)
    {
        toCreate = Point{pointToOpen.x + direction[i].x, pointToOpen.y + direction[i].y};
        if (!inBarrierAndCloseList(toCreate)) {
            openlist.push(createOpenPoint(toCreate, end, pointToOpen.cost + 1, &pointToOpen));
            closeAndBarrierList[toCreate.x][toCreate.y] = true;
        }
    }
}

//开始搜索路径
OpenPoint* findway(const Point& start, const Point& end) {
    //deepth = 0;
    // 创建并开启一个父节点
    openlist.push(createOpenPoint(start, end, 0, nullptr));
    closeAndBarrierList[start.x][start.y] = false;
    // 重复寻找预测和花费之和最小节点开启检查
    while (!openlist.empty())
    {
        auto toOpen = openlist.top();
        // 找到终点后返还end点
        if (*toOpen == end)
            return toOpen->father;
        //若超出一定深度(1000深度)，则失败
        //else if (deepth >= 1000)
            //return nullptr;
        open(*toOpen, start , end);
    }
    return nullptr;
}
//----------------------------------------------------------GREED---------------------------------------------------------------
//检查函数 返还成功与否值
inline bool inBarrier(std::pair<int,int> pos) {
    if (pos.first < 0 || pos.second < 0 || pos.first >= map_size || pos.second >= map_size)
        return true;
    return mp[pos.first][pos.second] != '.' && mp[pos.first][pos.second] != 'B';
}
int greed_next(std::pair<int,int> start , std::pair<int,int> end){
    //检查p点四方的点
    for (int i = 0; i < 4; ++i)
    {
        int tempx = start.first + px[i], tempy = start.second + py[i];
        if (!inBarrier({tempx,tempy})) {
            return i;
        }
    }
    return -1;
}


#endif