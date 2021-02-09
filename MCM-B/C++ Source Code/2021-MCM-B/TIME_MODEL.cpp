#include "TIME_MODEL.h"
#include <cstring>
#include <set>
TIME_MODEL::TIME_MODEL() {
    srand((unsigned)time(0));
}

void TIME_MODEL::read_known_data(vector<Fire> _Fires)
{
    sum_area = 0;
    Fires = _Fires;
    for(int i = 0;i < Fires.size();i++) sum_area += Fires[i].area;
}

double TIME_MODEL::distance(const Fire &a, const Fire &b)
{
    double dy = a.latitude-b.latitude;
    double dx = a.longitude-b.longitude;
    return sqrt(dx*dx+dy*dy)*100;
}

void TIME_MODEL::set_EOC(vector<EOC> _EOCS)
{
    EOCS = _EOCS;
}

double TIME_MODEL::distance(const EOC &a, double x, double y)
{
    double dy = a.latitude-y;
    double dx = a.longitude-x;
    return sqrt(dx*dx+dy*dy)*100;
}
double TIME_MODEL::distance(const Fire &a, double x, double y)
{
    double dy = a.latitude-y;
    double dx = a.longitude-x;
    return sqrt(dx*dx+dy*dy)*100;
}

bool TIME_MODEL::strict(double x, double y)
{
    double minv = 1e9,dis;
    for(int i = 0;i < Fires.size();i++)
    {
        dis = distance(Fires[i],x,y);
        if(dis < minv) minv = dis;
    }
    return dis < 100;

}
Fire TIME_MODEL::random_Fire()
{
    Fire newFire;
    double x,y,r;
    do
    {
        x = 140+rand()%10+(rand()*1.0/(double)RAND_MAX);
        y = -34-rand()%4-(rand()*1.0/(double)RAND_MAX);
    }while(!(x > 140.952893
       && y > -0.108776*x-22.714517
       && y > 0.278158*x-79.223946
       && y < -0.387852*x+20.658908));
    newFire.longitude = x;
    newFire.latitude = y;
    newFire.area = 0;
    newFire.value = 0;
    newFire.frequency = rand()%20;
    for(int i = 0;i < Fires.size();i++)
    {
        double dis = distance(newFire,Fires[i]);
        //if(dis > 50) continue;
        newFire.area += (2+rand()%3)*1.0/(1+dis)*Fires[i].area;
        newFire.value += (2+rand()%3)*1.0/(1+dis)*Fires[i].value;
        newFire.frequency += (2+rand()%3)*1.0/(1+dis)*Fires[i].frequency;
    }
    //cout<<"area = "<<newFire.area<<endl;
    //cout<<"value = "<<newFire.value<<endl;
    //cout<<"frequency = "<<newFire.frequency<<endl;
    r = (80+rand()%40)*1.0/100;
    newFire.area *= r;
    r = (90+rand()%20)*1.0/100;
    newFire.value *= r;
    int bigFire = rand()%((int)(newFire.frequency/20)+2);
    int midFire = rand()%((int)(newFire.frequency/5)+5);
    int smallFire = rand()%(int(newFire.frequency/2)+10);
    int day = 1;
    for(int i = 0;i < bigFire && day < 365;i++)
    {
        int days = 15+rand()%60;
        newFire.ft.emplace_back(fire_time(day,day+days,-1));
        int breakDays = 3+rand()%60;
        day += days+breakDays;
    }
    for(int i = 0;i < midFire && day < 365;i++)
    {
        int days = 5+rand()%10;
        newFire.ft.emplace_back(fire_time(day,day+days,-1));
        int breakDays = 3+rand()%30;
        day += days+breakDays;
    }
    for(int i = 0;i < smallFire && day < 365;i++)
    {
        int days = 1+rand()%5;
        newFire.ft.emplace_back(fire_time(day,day+days,-1));
        int breakDays = 3+rand()%20;
        day += days+breakDays;
    }
    /*
    cout<<"smallFire = "<<smallFire<<endl;
    cout<<"midFire = "<<midFire<<endl;
    cout<<"bigFire = "<<bigFire<<endl;
    cout<<"x = "<<newFire.longitude<<endl;
    cout<<"y = "<<newFire.latitude<<endl;
    cout<<newFire.frequency<<endl;
    cout<<newFire.area<<endl;
    cout<<newFire.value<<endl;

    for(int i = 0;i < newFire.ft.size();i++)
    {
        printf("[%d,%d] : %d\n",newFire.ft[i].left,newFire.ft[i].right,newFire.ft[i].count);
    }*/
    return newFire;
}

void TIME_MODEL::update_to_next_year()
{
    double r = (90+rand()%20)*1.0/100;
    double new_sum_area = sum_area*r;
    vector<Fire> new_Fires;
    sum_area = new_sum_area;
    r = (95+rand()%20)*1.0/100;
    int new_count = Fires.size()*r;
    for(int i = 0;i < new_count && new_sum_area > 0;i++)
    {

        Fire new_Fire = random_Fire();
        new_Fires.push_back(new_Fire);
        new_sum_area -= new_Fire.area;
    }
    Fires = new_Fires;
}

double TIME_MODEL::distance(const Fire& a,const EOC& b)
{
    double dy = a.latitude-b.latitude;
    double dx = a.longitude-b.longitude;
    return sqrt(dx*dx+dy*dy);
}

vector<double> TIME_MODEL::grade_model()
{

    int vis[15][500];
    memset(vis,0,sizeof(vis));
    int sum_mark = 0;
    for(int i = 0;i < Fires.size();i++)
    {
        int target = 0;
        double minv = distance(Fires[i],EOCS[0]);
         for(int j = 1;j < 15;j++)
         {
             double dis = distance(Fires[i],EOCS[j]);
             if(dis < minv)
             {
                 minv = dis;
                 target = j;
             }
         }
        if(minv > 0.8) continue;
        Fires[i].calculate_max_fire_time();
        int need;
        if(Fires[i].max_fire_time > 1) need = ((int)(Fires[i].value/30)+1)*3;
        else need =  ((int)(Fires[i].value/30)+1)*2;
        if(need > EOCS[target].count_of_SSA) continue;
        sum_mark++;
        for(int j = 0;j < Fires[i].ft.size();j++)
        {
            for(int k = Fires[i].ft[j].left;k <= Fires[i].ft[j].right;k++)
            {
                vis[target][k]++;
            }
        }
    }
    double hit = sum_mark*1.0/Fires.size();
    int workdays[15] = {0};
    int miss_day = 0;
    set<int> a;
    for(int i = 0;i < 15;i++)
    {
        for(int j = 1;j < 366;j++)
        {
            if(vis[i][j]) workdays[i]++;
            if(vis[i][j]*3 > EOCS[i].count_of_SSA) a.insert(j);
        }
    }
    miss_day = a.size();

    double final_result = hit*50;
    cout<<"hit = "<<hit<<endl;
    double sum = 0;
    for(int i = 0;i < 15;i++)
    {
        sum += workdays[i];
        cout<<"EOC["<<i+1<<"] work days : "<<workdays[i]<<endl;
        final_result += (workdays[i]*1.0/365)*20/15;
    }
    final_result += (365-miss_day)*1.0/365*30;
    cout<<"miss_days = "<<miss_day<<endl;
    cout<<"Final Mark = "<<final_result<<endl;
    return {hit,sum/365/15,miss_day*1.0/365,final_result};
}
