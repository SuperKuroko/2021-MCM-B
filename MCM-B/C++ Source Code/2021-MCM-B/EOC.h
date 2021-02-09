
#ifndef INC_2021_MCM_B_EOC_H
#define INC_2021_MCM_B_EOC_H
#include "Fire.h"
#include "Node.h"

class EOC {
public:
    //class members
    vector<Fire> Fires;
    int count_of_SSA;
    int count_of_Repeater;
    double latitude;
    double longitude;

    //class functions
    EOC();
    void set_location(pair<double,double> pos);
    void calculate_Repeater();
    void calculate_SSA();
    void Consider_Turn();
    void show();
};


#endif //INC_2021_MCM_B_EOC_H
