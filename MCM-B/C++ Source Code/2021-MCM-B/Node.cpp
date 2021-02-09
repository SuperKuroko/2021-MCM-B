#include "Node.h"

Node::Node()
{
    latitude = longitude = brightness = 0;
    for(int i = 0;i < 3;i++) date[i] = 0;
    times = fre = 0;
}

Node::Node(string s)
{
    int n = s.size();
    stringstream ss;

    //replace all comma with blank
    for(int i = 0;i < n;i++)
        if(s[i] == ',')
            s[i] = ' ';

    ss<<s;
    int count = 0;
    //latitude,longitude,brightness,scan,track,acq_date,acq_time,satellite,instrument,
    // confidence,version,bright_t31,frp,daynight,type
    while(ss>>s)
    {
        if(count == 0) latitude = stod(s);
        else if(count == 1) longitude = stod(s);
        else if(count == 2) brightness = stod(s);
        else if(count == 5)
        {
            date[0] = stoi(s.substr(0,4));
            date[1] = stoi(s.substr(5,2));
            date[2] = stoi(s.substr(8,2));
        }
        else if(count == 6) times = stoi(s);
        else if(count == 12)
        {
            fre = stod(s);
            return ;
        }
        count++;
    }
    ss.clear();
}

Node Node::getNode(string s)
{
    int n = s.size();
    stringstream ss;

    //replace all comma with blank
    for(int i = 0;i < n;i++)
        if(s[i] == ',')
            s[i] = ' ';

    ss<<s;
    int count = 0;
    Node result;
    //latitude,longitude,brightness,date,time,fre
    while(ss>>s)
    {
        if(count == 0) result.latitude = stod(s);
        else if(count == 1) result.longitude = stod(s);
        else if(count == 2) result.brightness = stod(s);
        else if(count == 3)
        {
            result.date[0] = stoi(s.substr(0,4));
            s = s.substr(5);
            result.date[1] = stoi(s.substr(0,s.find("-")));
            s = s.substr(s.find("-")+1);
            result.date[2] = stoi(s.substr(0));
        }
        else if(count == 4) result.times = stoi(s);
        else if(count == 5) result.fre = stod(s);
        count++;
    }
    ss.clear();
    return result;
}

void Node::show()
{
    printf("latitude  :%.4f\n",latitude);
    printf("longitude :%.4f\n",longitude);
    printf("brightness:%.1f\n",brightness);
    printf("date:%04d-%02d-%02d\n",date[0],date[1],date[2]);
    printf("time:%d\n",times);
    printf("fre:%.1f\n",fre);
}
