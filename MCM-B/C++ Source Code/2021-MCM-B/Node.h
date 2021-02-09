#ifndef INC_2021_MCM_B_NODE_H
#define INC_2021_MCM_B_NODE_H
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

/*
 * class node is used to store the keywords of a pixel.
 * as the csv file format, we split the string and extract
 * the information that related to the fire
 */
class Node {
public:
    //class member
    double latitude;   //latitude of pixel
    double longitude;  //longitude of pixel
    double brightness; //The temperature of the fire pixel
    int date[3];       //date[0] is year, date[1] is month, date[2] is day
    int times;         //acq_time row
    double fre;        //Fire radiative power
    int tag;           //belong to which cluster
    double angle;      //polar angle
    double dis;        //polar coordinates
    //as for other properties, are not relative of this lab,so we don't pay attention to them.

    //class function
    Node();          //default constructor
    Node(string s);  //constructor by using csv file's string
    static Node getNode(string s);
    void show();     //debug function, using to print a node's all member

};


#endif //INC_2021_MCM_B_NODE_H
