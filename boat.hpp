#ifndef __BOAT_H__
#define __BOAT_H__
#include "config.hpp"
#include "berth.hpp"
#include "func.hpp"
int collect_time[5] = {0};         // 存储五艘船装完完当前码头货物的时间
vector<pair<int, int>> berth_list; // 存储可去码头
int force_leave_time = 100;        // 超过这个帧数没有动作就去卖货
class Boat
{
public:
    Boat() {}
    Boat(int, int);
    int id;                               // 船的id
    int capacity;                         // 船的容量
    int status;                           // 0：运输中;1：正常运行状态;2：泊位外等待状态
    int target;                           // 目标泊位
    int exist_goods_num = 0;              // 船上的货物数
    int leave_time = -1;                  // 离开的时间，即货物装满的时间
    int zhen_id = -1;                     // 船进来的帧
    int no_action_time = 0;               // 船没有动作的时间
    void to_berth(int _target, int zhen); // 送去泊位
    void sold();                          // 卖货
    int judge_full(int zhen);             // 判断是否装满(当前码头货物能装满船则记录船离开时间并返回0；反之则返回传输完当前所有货物时的帧)
    void action(int zhen);
};

Boat boat[boat_num + 5];
Boat::Boat(int _cap, int _pos) : capacity(_cap), target(_pos) {}

void Boat::to_berth(int _target, int zhen)
{
    zhen_id = zhen + berth[_target].transport_time;
    berth[_target].status = 1;
    printf("ship %d %d\n", this->id, _target);
}

void Boat::sold()
{
    this->exist_goods_num = 0;
    this->leave_time = -1;
    this->zhen_id = -1;
    berth[this->target].status = 0;
    printf("go %d\n", this->id);
}

int Boat::judge_full(int zhen) // 仅在船刚到码头时与码头上货物收集完时调用
{
    int require_good = this->capacity - this->exist_goods_num; // 装满所需货物
    if (require_good <= berth[target].goods_num)               // 能装满
    {
        int time = require_good / berth[target].loading_speed;
        this->leave_time = zhen + time;
        berth[target].goods_num -= time * berth[target].loading_speed;
        this->no_action_time = 0;
        return 0;
    }
    else // 装不满
    {
        if(berth[target].goods_num == 0)
        {
            if(no_action_time < force_leave_time)  // 没到强制离开时间
            {
                no_action_time += 1;
                return zhen + 1;
            }
            else // 到了就强制离开去卖货
            {
                boat[this->id].sold();
                return 0;
            }
        }
        else
        {
            int time = berth[target].goods_num / berth[target].loading_speed;
            this->exist_goods_num += time * berth[target].loading_speed;
            berth[target].goods_num -= time * berth[target].loading_speed;
            this->no_action_time = 0;
            return zhen + time;
        }
        
    }
}

void Boat::action(int zhen)
{
    if (this->status == 1) // 在泊位或在虚拟点
    {
        if (this->target == -1) // 在虚拟点
        {
            int boat_flag = 0;
            for (int i = 0; i < berth_num; i++) // 遍历所有泊位
            {
                if (berth[i].status == 0) // 泊位上没船
                {
                    if (berth[i].goods_num != 0) // 泊位上有货
                    {
                        berth_list.push_back({i, berth[i].transport_time});
                    }
                }
                if (berth_list.size() != 0) // 有可去的泊位
                {
                    sort(berth_list.begin(), berth_list.end(), berth_compare); // 泊位的权重，目前根据已有货物数量和泊位与虚拟点的距离来判断(待改进：货物总价值，待送货物总价值，泊位装载速度)
                    int berth_target = berth_list[0].first;
                    this->to_berth(berth_target, zhen); // 送去泊点
                    boat_flag = 1;
                }
            }
            if (!boat_flag) // 没有有货且空余的泊位
            {
                vector<pair<int, int>> berth_list1; // 存储可去码头
                for (int i = 0; i < robot_num; i++) // 看看机器人是否有要去的码头
                {
                    if (robot[i].target_pull != -1 && robot[i].goods == 1 && berth[robot[i].target_pull].status == 0)
                    {
                        berth_list1.push_back({i,berth[robot[i].target_pull].transport_time});
                    }
                }
                if (berth_list1.size() != 0) // 有可去的泊位
                {
                    sort(berth_list1.begin(), berth_list1.end(), berth_compare); // 泊位的权重，目前根据已有货物数量和泊位与虚拟点的距离来判断(待改进：货物总价值，待送货物总价值，泊位装载速度)
                    int robot_target = berth_list1[0].first;
                    this->to_berth(robot[robot_target].target_pull, zhen); // 送去泊点
                }
                
            }
        }
        else // 在泊位，要判断是否装满
        {
            if (this->leave_time < zhen) // 没装满
            {
                if (this->zhen_id == zhen) // 船刚到泊位
                {
                    berth[this->target].status = 1;
                    int lodging_time = this->judge_full(zhen); // 判断港口货物能否装满
                    if (lodging_time != 0)
                    {
                        collect_time[this->id] = lodging_time;
                    }
                }
                else // 到了但没装满
                {
                    if (collect_time[this->id] == zhen) // 当前码头的货物都装满了
                    {
                        int lodging_time = boat[this->id].judge_full(zhen); // 判断港口货物能否装满
                        if (lodging_time != 0)
                        {
                            collect_time[this->id] = lodging_time;
                        }
                    }
                }
            }
            else // 装满了
            {
                boat[this->id].sold();
            }
        }
    }
    else if (boat[this->id].status == 2) // 泊位外等待
    {
        return; // 暂时不加也无妨
    }
}

#endif