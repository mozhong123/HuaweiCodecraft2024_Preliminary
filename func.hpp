#ifndef __FUNC_H__
#define __FUNC_H__
#include <bits/stdc++.h>
#include "robot.hpp"
#include "berth.hpp"
#include "goods.hpp"
#include "config.hpp"
using namespace std;

// int goodsbfs(int rest,  Goods *good)
// {
//     if (rest == 0) // 无可用机器人则返回-1
//         return -1;
//     queue<pair<int, int>> q;
//     q.push(make_pair(good->x, good->y));
//     bool r = 0, p = 0;
//     bool vis[201][201];
//     memset(vis, 0, sizeof(vis));
//     map<pair<int, int>, pair<int, int>> path;
//     pair<int, int> start, end;
//     path[q.front()] = make_pair(-1, -1);
//     Robot *tmp = nullptr;
//     Berth *tmpb = nullptr;
//     while (!q.empty())
//     {
//         pair<int, int> cur = q.front();
//         q.pop();
//         for (int i = 0; i < 3; i++)
//         {
//             int xx = cur.first + px[i], yy = cur.second + py[i];
//             // 不是障碍物
//             if (mp[xx][yy] != '*' && mp[xx][yy] != '#' && !vis[xx][yy]) //(这里应该改为机器人set中的搜索)
//             {
//                 if (mp[xx][yy] == 'A')
//                 {
//                     if (!r)
//                     { // 如果还没确定机器人
//                         // 如果机器人没有被选择，且不在恢复，且没有在运输则选择
//                         tmp = robot_find(xx, yy);
//                         if (!tmp->chosed && !tmp->goods && tmp->status)
//                         {
//                             r = 1;
//                             // 此处id应该为物品的索引？或者物品指针
//                             tmp->target_get = good;
//                             tmp->chosed = 1;
//                             start = make_pair(xx, yy); // 记录机器人在数组中的位置
//                             // 确定之后该位置就可以视为非障碍物
//                             q.push(make_pair(xx, yy));
//                             // 剩余机器人数量-1
//                             Robot::robot_rest--;
//                         }
//                         else
//                             tmp = nullptr;
//                     }
//                 }
//                 if (mp[xx][yy] == 'B')
//                 {
//                     if (!p)
//                     {
//                         // 未确定泊点则确定泊点
//                         tmpb = berth_find(xx,yy);
//                         p = 1;
//                         end = make_pair(xx, yy);
//                     }
//                     // 泊点可以视为空地
//                     q.push(make_pair(xx, yy));
//                 }
//                 if (mp[xx][yy] == '.')
//                     // 空地
//                     q.push(make_pair(xx, yy));
//                 path[make_pair(xx, yy)] = cur;
//                 vis[xx][yy] = 1;
//                 if (p && r)
//                 {
//                     // 此处id应为berth的id
//                     tmp->target_pull = tmpb;
//                     // 都找到则停止
//                     break;
//                 }
//             }
//         }
//         if (p && r)
//             break;
//     }
//     // 没找到则返回-1
//     if (!p || !r)
//         return -1;
//     // 寻找路径
//     // 取货路径
//     queue<int> op; // 0右，1左，2上，3下，4取，5放
//     pair<int, int> last = path[start];
//     while (last.first != -1)
//     {
//         if (last.first > start.first)
//             op.push(0);
//         else if (last.first < start.first)
//             op.push(1);
//         else if (last.second > start.second)
//             op.push(3);
//         else
//             op.push(2);
//         start = last;
//         last = path[start];
//     }
//     op.push(4);
//     // 放货路径
//     stack<int> reverse_op;
//     last = path[end];
//     reverse_op.push(5);
//     while (last.first != -1)
//     {
//         if (last.first > end.first)
//             reverse_op.push(1);
//         else if (last.first < end.first)
//             reverse_op.push(0);
//         else if (last.second < end.second)
//             op.push(2);
//         else
//             op.push(3);
//         end = last;
//         last = path[end];
//     }
//     while (!reverse_op.empty())
//     {
//         op.push(reverse_op.top());
//         reverse_op.pop();
//     }
//     tmp->op = op;
//     // 找到返回0
//     return 1;
// }

bool check_get(Robot& robot,Goods& good){
    if(good.x == robot.x && good.y == robot.y){
        printf("get %d %d\n", rand() % 4); // 修改
    }
}

bool check_pull(Robot& robot,Berth& berth){
    if(berth.x == robot.x && berth.y == robot.y){
        printf("pull %d %d\n", rand() % 4); // 修改
    }
}

#endif