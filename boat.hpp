#ifndef __BOAT_H__
#define __BOAT_H__
#include "config.hpp"
#include "berth.hpp"
#include "func.hpp"
#include <stdio.h>
int collect_time[5] = {0};                 // 存储五艘船装完完当前码头货物的时间
vector<pair<int, float>> berth_list;       // 存储可去码头
int force_leave_time = 200;                // 超过这个帧数没有动作就去卖货
FILE *logFile = fopen("logfile.txt", "w"); // 调试用的
// fprintf(logFile, "%d船舶有%d货物\n", target,berth[target].goods_num);
vector<int> berth_goods[berth_num]; // 存储每个码头到的货物价值
class Boat
{
public:
    Boat() {}
    Boat(int, int);
    int id;                  // 船的id
    int capacity;            // 船的容量
    int rest;                // 剩余容量
    int status;              // 0：运输中;1：正常运行状态;2：泊位外等待状态
    int target;              // 目标泊位
    int exist_goods_num = 0; // 船上的货物数
    int leave_time = -1;     // 离开的时间，即货物装满的时间
    int zhen_id = -1;        // 船进来的帧
    int no_action_time = 0;  // 船没有动作的时间
    int wait = 0;            // 还要等待多久
    int load_time = 0;       // 还要装多久
    int reach_time = 0;      // 还要多久才到
    int tot_value = 0;
    void to_berth(int _target, int zhen); // 送去泊位
    void sold();                          // 卖货
    int judge_full(int zhen);             // 判断是否装满(当前码头货物能装满船则记录船离开时间并返回0；反之则返回传输完当前所有货物时的帧)
    void action(int zhen);
    void action2(int zhen);
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
        for (int i = 0; i < time * berth[target].loading_speed; i++)
        {
            berth[target].goods_value -= berth_goods[target][i];
        }
        berth_goods[target].erase(berth_goods[target].begin(), berth_goods[target].begin() + time * berth[target].loading_speed);
        this->no_action_time = 0;
        return 0;
    }
    else // 装不满
    {
        if (berth[target].goods_num == 0)
        {
            if (this->no_action_time < force_leave_time) // 没到强制离开时间
            {
                this->no_action_time += 1;
                return zhen + 1;
            }
            else // 到了就强制离开去卖货
            {
                if (this->exist_goods_num != 0) // 收集到了货物
                {
                    this->sold();
                    return 0;
                }
                else // 没收集到货物
                {
                    this->no_action_time += 1;
                    return zhen + 1;
                }
            }
        }
        else
        {
            int time = 1;
            if (berth[target].goods_num < berth[target].loading_speed) // 货物不够一帧装的
            {
                this->exist_goods_num += berth[target].goods_num;
                for (int i = 0; i < berth[target].goods_num; i++)
                {
                    berth[target].goods_value -= berth_goods[target][i];
                }
                berth_goods[target].erase(berth_goods[target].begin(), berth_goods[target].begin() + berth[target].goods_num);
                berth[target].goods_num = 0;
            }
            else
            {
                time = berth[target].goods_num / berth[target].loading_speed;
                this->exist_goods_num += time * berth[target].loading_speed;
                berth[target].goods_num -= time * berth[target].loading_speed;
                for (int i = 0; i < time * berth[target].loading_speed; i++)
                {
                    berth[target].goods_value -= berth_goods[target][i];
                }
                berth_goods[target].erase(berth_goods[target].begin(), berth_goods[target].begin() + time * berth[target].loading_speed);
            }
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
                        berth_list.push_back({i, float(berth[i].goods_value)});
                    }
                }
            }
            if (berth_list.size() != 0) // 有可去的泊位
            {
                sort(berth_list.begin(), berth_list.end(), berth_opposite_compare); // 泊位的权重，目前根据已有货物数量和泊位与虚拟点的距离来判断(待改进：货物总价值，待送货物总价值，泊位装载速度)
                int berth_target = berth_list[0].first;
                this->to_berth(berth_target, zhen); // 送去泊点
                boat_flag = 1;
            }
            berth_list.clear();
            if (!boat_flag) // 没有有货且空余的泊位
            {
                vector<pair<int, float>> berth_list1; // 存储可去码头
                int robot2berth_num[berth_num] = {0}; // 去各码头的机器人数量
                for (int i = 0; i < robot_num; i++)   // 看看机器人是否有要去的码头
                {
                    if (robot[i].target_pull != -1 && robot[i].goods == 1 && berth[robot[i].target_pull].status == 0)
                    {
                        if (zhen < 500)
                        {
                            berth_list1.push_back({i, float(berth[robot[i].target_pull].transport_time)}); // 权值，由去码头的机器人数量与航船时间构成
                        }
                        else
                        {
                            if (berth[robot[i].target_pull].goods_value != 0)
                            {
                                berth_list1.push_back({i, float(berth[robot[i].target_pull].goods_value)});
                            }
                        }

                        // robot2berth_num[robot[i].target_pull]++;
                    }
                }
                // for (int i = 0; i < berth_num; i++) // 看看机器人是否有要去的码头
                // {
                //     if (robot2berth_num[i] != 0)
                //     {
                //         float weight = 0.8 * float(2.0 / robot2berth_num[i]) + 0.2 * float(berth[robot[i].target_pull].transport_time / 1000.0);
                //         berth_list1.push_back({i, weight}); // 权值，由去码头的机器人数量与航船时间构成
                //     }
                //     robot2berth_num[i] = 0;
                // }
                if (berth_list1.size() != 0) // 有可去的泊位
                {
                    sort(berth_list1.begin(), berth_list1.end(), berth_compare); // 泊位的权重，目前根据已有货物数量和泊位与虚拟点的距离来判断(待改进：货物总价值，待送货物总价值，泊位装载速度)
                    int robot_target = berth_list1[0].first;
                    this->to_berth(robot[robot_target].target_pull, zhen); // 送去泊点
                }
                else
                {
                    if (zhen >= 13000)
                    {
                        for (int i = 0; i < berth_num; i++) // 遍历所有泊位
                        {
                            if (berth[i].goods_num != 0) // 泊位上有货
                            {
                                berth_list.push_back({i, float(berth[i].goods_value)});
                            }
                        }
                        if (berth_list.size() != 0) // 有可去的泊位
                        {
                            sort(berth_list.begin(), berth_list.end(), berth_opposite_compare); // 泊位的权重，目前根据已有货物数量和泊位与虚拟点的距离来判断(待改进：货物总价值，待送货物总价值，泊位装载速度)
                            int robot_target = berth_list[0].first;
                            this->to_berth(robot[robot_target].target_pull, zhen); // 送去泊点
                        }
                        // else
                        // {

                        // }
                    }
                }
                berth_list1.clear();
                berth_list.clear();
            }
        }
        else // 在泊位，要判断是否装满
        {
            if (zhen == 14999 - berth[this->target].transport_time - 1)
            {
                this->sold();
            }
            if (this->leave_time < zhen) // 没装满
            {
                if (this->zhen_id == zhen) // 船刚到泊位
                {
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
                this->sold();
            }
        }
    }
    else if (boat[this->id].status == 2) // 泊位外等待
    {
        return; // 暂时不加也无妨
    }
}

void Boat::action2(int zhen)
{
    if (status == 0)
        reach_time--; // 减少运输时间
    if (status == 2)
        wait--;
    if (status == 1) // 正常运行状态
    {
        // 1.到达虚拟点,则继续遍历泊点，挑选最优的泊点(单位时间收益最高)
        if (target == -1)
        {
            double vpt = 0.0;
            int choose = -1;
            for (int i = 0; i < berth_num; i++)
            {
                /*
                1. 不用等待的泊点
                2. 需要等待的泊点，则加上等待时长（即上一艘船的装货完成时间-该船的到达时间)
                ps:可能有多艘船选择一个泊点，如果同时选择一个泊点则先输入指令的先进入泊点，则后一个等待，可以用队列来计算等待时间
                */
                // 首先计算价值
                int cnt = min(capacity, berth[i].goods_num); // （需要通过计算当前船最多能装多少来决定总价值）
                double v = 1.0;

                // 然后计算总时间
                double tot_time = 0.0;
                // 1.运输时间
                tot_time += 2 * berth[i].transport_time;

                // 3.等待时间(稍微复杂一点)
                if (berth[i].boat_q.size() != 0) // 当前港口没船，不用等待
                    // 等于最后一艘船需要到达时间+到达之后的等待时间+装货时间 -  当前船的到达时间
                    tot_time += max(boat[berth[i].boat_q.back()].reach_time + boat[berth[i].boat_q.back()].wait + boat[berth[i].boat_q.back()].load_time - berth[i].transport_time, 0);
                cnt = min(cnt, (int)(14999 - zhen - tot_time) * berth[i].loading_speed);
                if (cnt <= 0)
                    continue; // 时间不够了就不去了,或者太少了也不去
                for (int j = 0; j < cnt; j++)
                    v += berth_goods[i][j];
                // 2.装载时间
                // 当前容量或者多少货物（需要考虑前面的船要带走多少货物）
                tot_time += (cnt + berth[i].loading_speed - 1) / berth[i].loading_speed;
                double tmp = v / (double)tot_time;
                if (tmp > vpt) // 能在规定时间内返程的，且单位时间价值高的
                {
                    vpt = tmp;
                    choose = i;
                }
            }
            if (choose != -1) // 选择到了
            {
                // 港口货物减少，港口情况变化，船的等待和装载时间以及到达时间
                // 1.货物减少
                int cnt = min(capacity, berth[choose].goods_num);

                // 2.船的相关更新
                reach_time = berth[choose].transport_time;
                wait = 0;
                if (berth[choose].boat_q.size() != 0)
                    wait = max(boat[berth[choose].boat_q.back()].reach_time + boat[berth[choose].boat_q.back()].wait + boat[berth[choose].boat_q.back()].load_time - reach_time, 0);
                cnt = min(cnt, (int)(14999 - zhen - 2 * reach_time - wait) * berth[choose].loading_speed);
                rest = capacity - cnt;
                berth[choose].goods_num -= cnt;
                tot_value = 0;
                for (int i = 0; i < cnt; i++)
                {
                    tot_value += berth_goods[choose][i];
                }
                berth_goods[choose].erase(berth_goods[choose].begin(), berth_goods[choose].begin() + cnt);
                load_time = (cnt + berth[choose].loading_speed - 1) / berth[choose].loading_speed;
                target = choose;
                // 3.港口情况变化
                berth[choose].boat_q.push_back(id);
                sort(berth[choose].boat_q.begin(),berth[choose].boat_q.end(),[&](int a,int b){
                    return boat[a].reach_time == boat[b].reach_time ? a < b : boat[a].reach_time < boat[b].reach_time;
                });
                printf("ship %d %d\n", id, choose);
            }
        }
        else
        {
            /*
            1.正在装货
            2.装货完成
            */
            if (load_time > 0)
            {
                load_time--;
            }
            else
            {
                // 装货完成
                // 判断后续有无船，有船等待就滚，没船等待就继续
                if (rest == 0)
                {
                    // 没容量，滚去卖货
                    berth[target].boat_q.erase(berth[target].boat_q.begin());
                    printf("go %d\n", id);
                }
                else
                {
                    int cnt = min(rest, berth[target].goods_num);
                    //如果时间不够运货那么就不选该泊点
                    cnt = min(cnt, (14999 - zhen - berth[target].transport_time) * berth[target].loading_speed);
                    if (cnt <= 0)
                    {
                        berth[target].boat_q.erase(berth[target].boat_q.begin());
                        printf("go %d\n", id);
                    }
                    else
                    {
                        if (berth[target].boat_q.size() == 1)
                        {
                            rest -= cnt;
                            load_time = (cnt + berth[target].loading_speed - 1) / berth[target].loading_speed - 1; // 向上取整，并减去当前帧的装货时间
                            berth[target].goods_num -= cnt;
                            for (int i = 0; i < cnt; i++)
                                tot_value += berth_goods[target][i];
                            berth_goods[target].erase(berth_goods[target].begin(), berth_goods[target].begin() + cnt);
                        }
                        else
                        {
                            // 有船则不再在当前处装货，腾出地方给其他人用，如果容量不到一半则继续运输
                            berth[target].boat_q.erase(berth[target].boat_q.begin());
                            if (rest >= (capacity / 3))
                            {
                                int pre_choose = -1;
                                double vpt = 0.0;
                                for (int i = 0; i < berth_num; i++)
                                {
                                    if (i == target)
                                        continue;
                                    int cnt = min(rest, berth[i].goods_num); // （需要通过计算当前船最多能装多少来决定总价值）
                                    // 然后计算总时间
                                    double tot_time = 0.0;
                                    // 1.运输时间
                                    tot_time += 2 * berth[i].transport_time;
                                    double v = 1.0;
                                    // 3.等待时间(稍微复杂一点)
                                    if (berth[i].boat_q.size() != 0) // 当前港口没船，不用等待
                                        // 等于最后一艘船需要到达时间+到达之后的等待时间+装货时间 -  当前船的到达时间
                                        tot_time += max(boat[berth[i].boat_q.back()].reach_time + boat[berth[i].boat_q.back()].wait + boat[berth[i].boat_q.back()].load_time - berth[i].transport_time, 0);
                                    cnt = min(cnt, (int)(14999 - zhen - tot_time) * berth[i].loading_speed);
                                    if (cnt <= 0)
                                        continue;
                                    for (int j = 0; j < cnt; j++)
                                        v += berth_goods[i][j];
                                    // 2.装载时间
                                    // 当前容量或者多少货物（需要考虑前面的船要带走多少货物）
                                    tot_time += (cnt + berth[i].loading_speed - 1) / berth[i].loading_speed;
                                    double tmp = v / (double)tot_time;
                                    if (tmp > vpt) // 能在规定时间内返程的，且单位时间价值高的
                                    {
                                        vpt = tmp;
                                        pre_choose = i;
                                    }
                                }
                                if (pre_choose != -1)
                                {
                                    int cnt = min(rest, berth[pre_choose].goods_num);
                                    // 2.船的相关更新
                                    reach_time = berth[pre_choose].transport_time;
                                    wait = 0;
                                    if (berth[pre_choose].boat_q.size() != 0)
                                        wait = max(boat[berth[pre_choose].boat_q.back()].reach_time + boat[berth[pre_choose].boat_q.back()].wait + boat[berth[pre_choose].boat_q.back()].load_time - reach_time, 0);
                                    cnt = min(cnt, (int)(14999 - zhen - 2 * reach_time - wait) * berth[pre_choose].loading_speed);
                                    rest -= cnt;
                                    berth[pre_choose].goods_num -= cnt;
                                    for (int i = 0; i < cnt; i++)
                                    {
                                        tot_value += berth_goods[pre_choose][i];
                                    }
                                    berth_goods[pre_choose].erase(berth_goods[pre_choose].begin(), berth_goods[pre_choose].begin() + cnt);
                                    load_time = (cnt + berth[pre_choose].loading_speed - 1) / berth[pre_choose].loading_speed;
                                    target = pre_choose;
                                    // 3.港口情况变化
                                    berth[pre_choose].boat_q.push_back(id);
                                    sort(berth[pre_choose].boat_q.begin(),berth[pre_choose].boat_q.end(),[&](int a,int b){
                                        return boat[a].reach_time == boat[b].reach_time ? a < b : boat[a].reach_time < boat[b].reach_time;
                                    });
                                    printf("ship %d %d\n", id, pre_choose);
                                }
                            }
                            else //剩余容量不多了
                            {
                                printf("go %d\n", id);
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif