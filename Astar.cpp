#include"Astar.h"

//创建地图
void createMap() {
    for (int i = 0; i < map_size; ++i)
        for (int j = 0; j < map_size; ++j){
            //五分之一概率生成障碍物，不可走
            if (rand() % 5 == 0) {
                mapBuffer[i][j] = '*';
                closeAndBarrierList[i][j] = true;
            }
            else {
                mapBuffer[i][j] = ' ';
                closeAndBarrierList[i][j] = false;
            }
        }
}

//打印地图
void printMap() {
    std::cout<<"打印地图"<<std::endl;
    for (int i = 0; i < map_size; ++i) {
        for (int j = 0; j < map_size; ++j)
            std::cout << mapBuffer[i][j];
        std::cout << std::endl;
    }
    std::cout<<"打印完毕"<<std::endl;
    std::cout << std::endl << std::endl << std::endl;
}

int main() {
    //起点
    Point begin = {0,0};
    //终点
    Point end = {189,189};
    //创建地图
    createMap();
    //保证起点和终点都不是障碍物
    mapBuffer[begin.x][begin.y] = mapBuffer[end.x][end.y] = ' ';
    closeAndBarrierList[begin.x][begin.y] = closeAndBarrierList[end.x][end.y] = false;
    //根据搜索得到的终点openpoint，一直延路径回溯
    //并将回溯经过的点标记为'O'
    for (auto rs = findway(Point{ begin.x,begin.y }, Point{ end.x,end.y }); rs != nullptr; rs = rs->father) {
        mapBuffer[rs->x][rs->y] = 'O';
    }
    //打印走过路后的地图
    printMap();
    system("pause");
    return 0;
}