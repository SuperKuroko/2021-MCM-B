#ifndef INC_2021_MCM_B_TIME_MODEL_H
#define INC_2021_MCM_B_TIME_MODEL_H
#include <ctime>
#include "EOC.h"
#include "Fire.h"
#include "Node.h"

#include <cmath>
class TIME_MODEL {
public:
    //int year;
    vector<Fire> origin_Fires;
    vector<Fire> Fires;
    vector<EOC> EOCS;
    double sum_area;

    TIME_MODEL();
    void read_known_data(vector<Fire> _Fires);
    Fire random_Fire();
    void update_to_next_year();
    vector<double> grade_model();
    void set_EOC(vector<EOC> _EOCS);
    double distance(const Fire& a,const Fire& b);
    double distance(const Fire& a,const EOC& b);
    double distance(const EOC&a, double x,double y);
    double distance(const Fire &a, double x, double y);
    bool strict(double x,double y);
};


#endif //INC_2021_MCM_B_TIME_MODEL_H
