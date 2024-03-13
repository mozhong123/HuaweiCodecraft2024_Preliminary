#include <bits/stdc++.h>
using namespace std;
char mp[201][201];

void goodsbfs(int rest)
{
    if (rest == 0)
        return;
    queue<pair<int, int>> q;
    q.push(make_pair(0, 0));
    int px[4] = {0, 0, 1, -1}, py[4] = {1, -1, 0, 0};
    bool r = 0, p = 0;
    bool vis[201][201];
    memset(vis, 0, sizeof(vis));
    map<pair<int, int>, pair<int, int>> path;
    pair<int, int> start, end;
    path[make_pair(0, 0)] = make_pair(-1, -1);
    while (!q.empty())
    {
        pair<int, int> cur = q.front();
        q.pop();
        for (int i = 0; i < 3; i++)
        {
            int xx = cur.first + px[i], yy = cur.second + py[i];
            // 不是障碍物
            if (mp[xx][yy] != '*' && mp[xx][yy] != '#' && !vis[xx][yy]) //(这里应该改为机器人set中的搜索)
            {
                if (mp[xx][yy] == 'A')
                {
                    if (!r)
                    { // 如果还没确定机器人，则选择当前机器人
                        r = 1;
                        start = make_pair(xx, yy); // 记录机器人在数组中的位置
                        // 确定之后该位置就可以视为非障碍物
                        q.push(make_pair(xx, yy));
                    }
                }
                else if (mp[xx][yy] == 'B')
                {
                    if (!p)
                    {
                        // 未确定泊点则确定泊点
                        p = 1;
                        end = make_pair(xx, yy);
                    }
                    // 泊点可以视为空地
                    q.push(make_pair(xx, yy));
                }
                else
                { // 空地
                    q.push(make_pair(xx, yy));
                }
                if (p && r) // 机器人和泊点都找到
                    return; // 找到泊点和机器人则停止
                path[make_pair(xx, yy)] = cur;
                vis[xx][yy] = 1;
            }
        }
    }
    // 寻找路径
    // 取货路径
    queue<int> op; // 0右，1左，2上，3下，4取，5放
    pair<int, int> last = path[start];
    while (last.first != -1)
    {
        if (last.first > start.first)
            op.push(0);
        else if (last.first < start.first)
            op.push(1);
        else if (last.second > start.second)
            op.push(3);
        else
            op.push(2);
        start = last;
        last = path[start];
    }
    op.push(4);
    // 放货路径
    stack<int> reverse_op;
    last = path[end];
    reverse_op.push(5);
    while (last.first != -1)
    {
        if (last.first > end.first)
            reverse_op.push(1);
        else if (last.first < end.first)
            reverse_op.push(0);
        else if (last.second < end.second)
            op.push(2);
        else
            op.push(3);
        end = last;
        last = path[end];
    }
    while(!reverse_op.empty())
    {
        op.push(reverse_op.top());
        reverse_op.pop();
    }
}


bool check_get(Robot& robot,vector <Goods>& goods){
    if(goods[robot.target_get].x == robot.x && goods[robot.target_get].y == robot.y){
        printf("get %d %d\n", rand() % 4); // 修改
    }
}

bool check_pull(Robot& robot,vector<Berth>& berth){
    if(berth[robot.target_pull].x == robot.x && berth[robot.target_pull].y == robot.y){
        printf("pull %d %d\n", rand() % 4); // 修改
    }
}