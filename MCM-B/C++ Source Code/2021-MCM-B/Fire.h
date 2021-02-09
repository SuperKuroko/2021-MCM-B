#ifndef INC_2021_MCM_B_FIRE_H
#define INC_2021_MCM_B_FIRE_H
#include "Node.h"
#include <unordered_set>
#include <unordered_map>
/*
 * class Fire is a kind of struct that describes the position, acreage
 * fire scale and fire frequency. A node is generated from series of pixels
 * by k-means method.
 */

struct fire_time
{
    int left;
    int right;
    int count;
    fire_time(){
        left = right = count = -1;
    }
    fire_time(int _left,int _right,int _count)
    {
        left = _left;
        right = _right;
        count = _count;
    }
};

class Fire {
public:
    //class members
    double latitude;    //latitude of pixel.
    double longitude;   //longitude of pixel.
    double frequency;   //the frequency fire in an area
    double value;       //describe the scale of fire, which is generated from bright,fire radiative power,the size of area and number of pixels
    vector<Node> nodes; // the nodes it contain
    vector<int> t;      // time segment
    int count;          // nodes number
    double area;        //the area value
    vector<pair<int,int>> dict;
    vector<fire_time> ft;
    int max_fire_time;
    double average_fire_time;

    //class functions
    Fire();
    Fire(vector<Node> _nodes);
    void calculate_max_fire_time();
    void show();
};


#endif //INC_2021_MCM_B_FIRE_H
