#ifndef __GoodsS_H__
#define __GoodsS_H__
class Goods
{
public:
    Goods() {}
    Goods(int, int, int, int);
    int x, y;
    int value;
    int zhen; // 帧id
    int chosed; // 是否被选择
};

Goods::Goods(int xx, int yy, int _value, int _zhen) : x(xx), y(yy), value(_value), zhen(_zhen) {}


#endif