#ifndef INC_2021_MCM_B_ALGORITHM_H
#define INC_2021_MCM_B_ALGORITHM_H
#include "Node.h"
#include "Fire.h"
#include "Print.h"
#include "EOC.h"
#include "TIME_MODEL.h"
#include <ctime>
#include <cmath>
#include <algorithm>
#include <stack>
#include <fstream>
#include <unordered_map>

const double pi = 3.1415926535898;  //the value of pi

struct MyStack  //stack structure
{
    int *d;                           //data
    int cnt = 0;                      //size
    void push(int x){d[cnt++] = x;}
    void pop(){cnt--;}
    bool empty(){return cnt == 0;}
    int top(){return d[cnt-1];}
    int sop(){return d[cnt-2];}        //second-top
    MyStack()
    {
        d = new int [10000];
        cnt = 0;
    }
    MyStack(int size)
    {
        d = new int [size];
        cnt = 0;
    }
};


double f(double x,double y)  //Transform Polar Angle function
{
    if(x == 0 && y > 0) return pi/2;
    if(x == 0 && y < 0) return 3*pi/2;
    if(x > 0 && y == 0) return 0;
    if(x < 0 && y == 0) return pi;

    if(x > 0 && y > 0) return atan(y/x);
    if(x > 0 && y < 0) return atan(y/x)+ 2*pi;
    if(x < 0 && y < 0) return atan(y/x)+ pi;
    if(x < 0 && y > 0) return atan(y/x)+ pi;
}

bool Node_cmp_lat(const Node &a, const Node &b)  //using latitude as sorting keywords
{
    if(a.latitude != b.latitude) return a.latitude < b.latitude;
    return a.longitude < b.longitude;
}

bool Node_cmp_ang(const Node& a, const Node& b)  //using polar angle as sorting keywords
{
    if(a.angle != b.angle) return a.angle < b.angle;
    return a.dis < b.dis;
}

bool Node_cmp_tag(const Node& a, const Node& b)  //using tag as sorting keywords
{
    return a.tag > b.tag;
}

bool Fire_cmp_area(const Fire& a,const Fire& b)
{
    return a.area < b.area;
}

bool dict_cmp_time(const pair<int,int> &a,const pair<int,int>&b)
{
    return a.first < b.first;
}

bool CrossProduct(const Node &left,const Node& mid,const Node& right)  //judge whether cross product >= 0
{
    double x1 = mid.longitude-left.longitude;
    double x2 = right.longitude-mid.longitude;
    double y1 = mid.latitude-left.latitude;
    double y2 = right.latitude-mid.latitude;
    return x2*y1-x1*y2 >= 0;
}

double distance(const Node& a,const Node& b)
{
    double dx = a.longitude-b.longitude;
    double dy = a.latitude-b.latitude;
    return sqrt(dx*dx+dy*dy);
}


double Graham(vector<Node>& cluster,int val = 0)
{
    if(cluster.size() < 3) return 0;
    int n = cluster.size();
    sort(cluster.begin(),cluster.end(),Node_cmp_lat);
    double dx = cluster[0].longitude,dy = cluster[0].latitude;
    for(int i = 0;i < n;i++)
    {
        cluster[i].longitude -= dx;
        cluster[i].latitude -= dy;
        cluster[i].dis = sqrt(pow(cluster[i].longitude,2)+pow(cluster[i].latitude,2));
        cluster[i].angle = f(cluster[i].longitude,cluster[i].latitude);
    }
    sort(cluster.begin(),cluster.end(),Node_cmp_ang);
    MyStack w(n);
    for(int i = 0;i < 3;i++) w.push(i);
    for(int i = 3;i < n;i++)
    {
        while(w.cnt > 2 && CrossProduct(cluster[w.sop()],cluster[w.top()],cluster[i])) w.pop();
        w.push(i);
    }
    for(int i = 0;i < n;i++) cluster[i].tag = -val;

    for(int i = 0;i < w.cnt;i++)
        cluster[w.d[i]].tag = val;
    //sort(cluster.begin(),cluster.end(),Node_cmp_tag);
    double area = sqrt(pow(cluster[w.d[0]].longitude-cluster[w.top()].longitude,2)+pow(cluster[w.top()].latitude-cluster[w.d[0]].latitude,2));
    for(int i = 1;i < w.cnt;i++)
        area += sqrt(pow(cluster[w.d[i]].longitude-cluster[w.d[i-1]].longitude,2)+pow(cluster[w.d[i]].latitude-cluster[w.d[i-1]].latitude,2));

    for(int i = 0;i < n;i++)
    {
        cluster[i].longitude += dx;
        cluster[i].latitude += dy;
    }
    return area;
}


vector<vector<Node>> K_means(int k,const vector<Node>& data)
{
    int n = data.size();
    vector<vector<Node>> result(k);
    //init
    srand((unsigned)time(0));  //random function
    Node *center = new Node [k];             //record the centre for each cluster
    bool *vis = new bool [n]();              //while firstly random select k numbers, record whether a node is visited
    int *id = new int [n]();                 //id[i] means the ith node belong to which cluster
    //random select k numbers to be centre
    for(int i = 0;i < k;i++)
    {
        int val;
        do
        {
            val = rand()%n;
        }while(vis[val]);
        vis[val] = true;
        center[i] = data[val];
    }

    while(true)
    {
        bool isChanged = false;
        for(int i = 0;i < k;i++) result[i].clear(); //clear
        for(int i = 0;i < n;i++)
        {
            double minv = distance(data[i],center[0]); // minimum value
            int target = 0;                               // target cluster number
            for(int j = 1;j < k;j++)                      //search minimum distance
            {
                double dis = distance(data[i],center[j]);
                if(dis < minv)
                {
                    minv = dis;
                    target = j;
                }
            }
            if(id[i] != target) isChanged = true;
            id[i] = target;
            result[target].push_back(data[i]);            //add to target cluster
        }
        if(!isChanged) break;    //if every node's cluster number is not changed, it means algorithm is over.

        //or we should use the new clusters' average value to replace previous centre
        for(int i = 0;i < k;i++)
        {
            center[i].latitude = center[i].longitude = 0;
            for(int j = 0;j < result[i].size();j++)
            {
                center[i].latitude += result[i][j].latitude;
                center[i].longitude += result[i][j].longitude;
            }
            center[i].latitude /= result[i].size();
            center[i].longitude /= result[i].size();
        }
    }
    return result;
}

vector<vector<Node>> finalResult;
void recursive_K_Means(vector<vector<Node>> clusters,int depth)
{
    for(int i = 0;i < clusters.size();i++)
    {

        double area = Graham(clusters[i]);
        //cout<<"area = "<<area<<",depth = "<<depth<<endl;
        if(area > 1.8 && depth < 5)
        {
            vector<vector<Node>> splitClusters = K_means(int(area)+1,clusters[i]);
            recursive_K_Means(splitClusters,depth+1);
        }
        else if(area > 0.8) finalResult.push_back(clusters[i]);
    }

}

vector<vector<Node>> Model_I_part1(int k,const vector<Node>& data)
{
    finalResult.clear();
    vector<vector<Node>> clusters = K_means(k,data);
    recursive_K_Means(clusters,0);
    return finalResult;
}

vector<pair<double,double>> add_EOC()
{
    vector<pair<double,double>> EOCS(15);
    freopen("EOC.txt","r",stdin);
    for(int i = 0;i < 15;i++)
        cin>>EOCS[i].first>>EOCS[i].second;
    return EOCS;
}


void Model_I_part2(vector<Fire> & Fires)
{
    unordered_map<int,int> dict;
    for(Fire& fire:Fires)
    {
        dict.clear();
        sort(fire.t.begin(),fire.t.end());
        for(int i = 0;i < fire.t.size();i++)
        {
            if(dict.count(fire.t[i])) dict[fire.t[i]]++;
            else dict[fire.t[i]] = 1;
        }
        int index = 0;
        for(auto it = dict.begin();it != dict.end();it++)
        {
            fire.dict.emplace_back(make_pair(it->first,it->second));
        }
        sort(fire.dict.begin(),fire.dict.end(),dict_cmp_time);
        int left = fire.dict[0].first;
        int count = fire.dict[0].second;
        int right = left;
        for(int i = 1;i < fire.dict.size();i++)
        {
            if(fire.dict[i].first-fire.dict[i-1].first < 1000)
            {
                count += fire.dict[i].second;
                right = fire.dict[i].first;
            }
            else
            {
                fire.ft.emplace_back(fire_time(left,right,count));
                left = right = fire.dict[i].first;
                count = fire.dict[i].second;
            }
        }
        fire.ft.emplace_back(fire_time(left,right,count));
        fire.max_fire_time = 0;
        fire.average_fire_time = 0;
        for(int i = 0;i < fire.ft.size();i++)
        {
            fire.max_fire_time = max(fire.max_fire_time,fire.ft[i].right-fire.ft[1].left);
            fire.average_fire_time += (double)(fire.ft[i].right-fire.ft[1].left);
        }
        fire.average_fire_time /= fire.ft.size();
    }
}

double distance(const Fire& a,const pair<double,double> b)
{
    double dy = a.latitude-b.first;
    double dx = a.longitude-b.second;
    return sqrt(dx*dx+dy*dy);
}


vector<vector<Fire>> merge_Fires(const vector<pair<double,double>> &EOCS,const vector<Fire>& original_Fires)
{
    vector<vector<Fire>> group(15);
    for(int i = 0;i < original_Fires.size();i++)
    {
        int min_distance = distance(original_Fires[i],EOCS[0]);
        int target = 0;
        for(int k = 1;k < 15;k++)
        {
            int value = distance(original_Fires[i],EOCS[k]);
            if(value < min_distance)
            {
                min_distance = value;
                target = k;
            }
        }
        group[target].push_back(original_Fires[i]);
    }
    return group;
}

vector<EOC> Model_I_part3(vector<vector<Fire>> merged_Fires,vector<pair<double,double>> EOC_pos)
{
    vector<EOC> EOCS(15);
    for(int i = 0;i < 15;i++)
    {
        EOCS[i].Fires = merged_Fires[i];
        EOCS[i].set_location(EOC_pos[i]);
        EOCS[i].calculate_Repeater();
        EOCS[i].calculate_SSA();
        EOCS[i].Consider_Turn();
        EOCS[i].show();
    }
    return EOCS;
}

void write_forecast_data(int year,vector<Fire> Fires)
{

}

void Model_II(vector<Fire>Fires, vector<EOC> EOCS)
{
    TIME_MODEL TM;
    TM.read_known_data(Fires);
    TM.set_EOC(EOCS);
    cout<<TM.Fires.size()<<endl;
    ofstream fout;
    fout.open("grade.csv",ios::out);
    fout<<"year,hit,work,miss,mark"<<endl;
    for(int i = 2020;i < 2030;i++)
    {
        cout<<"year:"<<i<<endl;
        TM.update_to_next_year();
        cout<<TM.Fires.size()<<endl;
        vector<double> val = TM.grade_model();
        cout<<endl;
        fout<<i<<","<<val[0]*100<<","<<val[1]*100<<","<<val[2]*100<<","<<val[3]/2<<endl;

    }
    fout.close();

}

#endif //INC_2021_MCM_B_ALGORITHM_H
