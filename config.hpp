#ifndef __CONFIG_H__
#define __CONFIG_H__
const int robot_num = 10;
const int berth_num = 10;
const int boat_num = 5;
const int map_size = 200;
//方向
int px[4] = {0, 0, -1, 1}, py[4] = {1, -1, 0, 0};
char mp[map_size+10][map_size+10];
#endif