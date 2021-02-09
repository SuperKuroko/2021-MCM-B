
#ifndef INC_2021_MCM_B_PRINT_H
#define INC_2021_MCM_B_PRINT_H
#include <vector>
#include <iostream>
#include "Fire.h"
using namespace std;
void Print(vector<pair<int,int>> a)
{
    for(int i = 0;i < a.size();i++)
    {
        cout<<a[i].first<<" : "<<a[i].second<<endl;
    }
}

void Print(fire_time ft)
{
    printf("[%d,%d] : %d\n",ft.left,ft.right,ft.count);
}

void Print(vector<fire_time>fts)
{
    for(int i = 0;i < fts.size();i++)
    {
        Print(fts[i]);
        if(i != 0)
        {
            cout<<(fts[i].left-fts[i-1].right)*1.0/(1440)<<endl;
        }
    }
}

#endif //INC_2021_MCM_B_PRINT_H
