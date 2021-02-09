#include "EOC.h"
#include <cmath>
#include <algorithm>
EOC::EOC()
{
    latitude = longitude = -1;
    count_of_Repeater = count_of_Repeater = 0;
}

void EOC::set_location(pair<double, double> pos)
{
    latitude = pos.first;
    longitude = pos.second;
}

void EOC::calculate_Repeater()
{
    int close = 0;
    vector<int> far;
    for(int i = 0;i < Fires.size();i++)
    {
        double dx = Fires[i].longitude-longitude;
        double dy = Fires[i].latitude-latitude;
        double dis = sqrt(dx*dx+dy*dy);
        if(dis*100 < 20) close++;
        else far.push_back(i);
    }
    if(far.empty()) count_of_Repeater = 1;
    else if(far.size() == 1) count_of_Repeater = 2;
    else if(far.size() == 2)
    {
        double x1 = Fires[far[0]].longitude-longitude;
        double y1 = Fires[far[0]].latitude-latitude;
        double x2 = Fires[far[1]].longitude-longitude;
        double y2 = Fires[far[1]].latitude-latitude;
        if(x1*x2+y1*y2 < 0) count_of_Repeater = 3;
        else count_of_Repeater = 2;
    }
    else if(far.size() == 3)
    {
        double x1 = Fires[far[0]].longitude-longitude;
        double y1 = Fires[far[0]].latitude-latitude;
        double x2 = Fires[far[1]].longitude-longitude;
        double y2 = Fires[far[1]].latitude-latitude;
        double x3 = Fires[far[2]].longitude-longitude;
        double y3 = Fires[far[2]].latitude-latitude;
        int k = 0;
        if(x1*x2+y1*y2 > 0) k++;
        if(x3*x2+y3*y2 > 0) k++;
        if(x1*x3+y1*y3 > 0) k++;
        if(k == 0) count_of_Repeater = 4;
        else if(k == 1 || k == 2) count_of_Repeater = 3;
        else if(k == 3) count_of_Repeater = 2;
    }
    else count_of_Repeater = 5;
}


void EOC::calculate_SSA()
{
    count_of_SSA = 0;
    for(int i = 0;i < Fires.size();i++)
    {
        if(Fires[i].max_fire_time > 225)
            count_of_SSA += ((int)(Fires[i].value/30)+1)*3;
        else
            count_of_SSA += ((int)(Fires[i].value/30)+1)*2;
    }
}

void EOC::Consider_Turn()
{
    int maxv = 0;
    for(int i = 0;i < Fires.size();i++)
        maxv = max(maxv,Fires[i].max_fire_time);
    if(maxv > 225) count_of_Repeater *= 3;
    else count_of_Repeater *= 2;
}

void EOC::show()
{
    cout<<"latitude = "<<latitude<<endl;
    cout<<"longitude = "<<longitude<<endl;
    cout<<"sum = "<<Fires.size()<<endl;
    cout<<"SSA = "<<count_of_SSA<<endl;
    cout<<"Repeater = "<<count_of_Repeater<<endl;

    cout<<endl;
}