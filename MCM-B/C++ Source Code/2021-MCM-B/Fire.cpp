#include "Fire.h"

Fire::Fire()
{
    latitude = longitude = frequency = value = 0;
    nodes.clear();
    area = 0;
    count = 0;
}

Fire::Fire(vector<Node> _nodes)
{
    nodes = _nodes;
    count = nodes.size();
    latitude = longitude = frequency = value = 0;
    for(int i = 0;i < count;i++)
    {
        longitude += nodes[i].longitude;
        latitude += nodes[i].latitude;
        value += nodes[i].brightness/10+nodes[i].fre/2;
    }
    value /= count;
    longitude /= count;
    latitude /= count;
    t.resize(count);
    unordered_set<int> uset;
    for(int i = 0;i < count;i++)
    {
        t[i] = (nodes[i].date[0]*365+nodes[i].date[1]*30+nodes[i].date[2])*24*60+nodes[i].times;
        if(!uset.count(t[i])) uset.insert(t[i]);
    }
    frequency = uset.size();

}

void Fire::calculate_max_fire_time()
{
    int ans = 0;
    for(int i = 0;i < ft.size();i++)
        ans = max(ans,ft[i].right-ft[i].left);
    max_fire_time = ans;
}

void Fire::show()
{

}