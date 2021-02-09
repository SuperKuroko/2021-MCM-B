#include <fstream>
#include "Node.h"
#include "Fire.h"
#include "EOC.h"
#include "Algorithm.h"
using namespace std;
//Node->Fire->EOC->TIME_MODEL
void read_data_from_origin_file();
void write_file_after_select();
void read_data_from_select_file();
void write_visual_data();
void write_visual_data_with_scale();
void generate_fire();
void delete_data();
double calculate_sum_area();

vector<Node> data;
vector<vector<Node>> clusters;
vector<Fire> Fires;
vector<pair<double,double>> EOC_pos;
vector<vector<Fire>> merged_Fires;
vector<EOC> EOCS;

void Model_I();
int k = 20;      //k-means algorithm's k value
const double sumArea = 22.8994;

int main()
{
    //delete_data();
    //sumArea = calculate_sum_area();
    //read_data_from_csv_file();
    //write_file_after_select();
    read_data_from_select_file();
    Model_I();
    Model_II(Fires,EOCS);

    //write_visual_data_with_scale();
    return 0;
}


void read_data_from_origin_file()
{
    /*
     * fire_archive_M6_156000.csv is a file that stores about 60000 pieces of data
     * for each of pieces, it has the format as follow:
     * latitude: Latitude of pixel.
     * longitude: Longitude of pixel.
     * brightness: The temperature of the fire pixel, in Kelvin.
     * scan: The width of the pixel at a particular location.
     * track: The height of the pixel at a particular location.
     * acq_date: The date the data for this pixel was acquired.
     * acq_time: The time where the data for this pixel was acquired.
     * satellite: Either aqua or terra.
     * instrument: Instrument the data was collected with.
     * confidence: Confidence of the fire at a pixel, from 0 (low-confidence of a fire) to 100 (high confidence of a fire).
     * version: the version of this piece
     * frp: Fire radiative power
     * daynight: day of night
     * type: the type of this piece
     */
    //ifstream fin("fire_archive_M6_156000.csv"); //read data from origin file
    //ifstream fin("fire_nrt_M6_156000.csv");     //read data from origin file
    ifstream fin("fire_nrt_V1_101674.csv");    //read data from origin file
    string line;
    getline(fin,line);
    while(getline(fin,line))
    {
        data.emplace_back(Node(line));
        double y = data.back().latitude;
        double x = data.back().longitude;
        /*
         * the data contains the whole Australia,
         * but what we pay attention to is only Victoria,
         * so we divide the Victoria into a quadrangle,
         * then for each side, we calculate the equation
         * to make linear programming, which can judge whether
         * a pixel is in Victoria
         */
        if(x > 140.952893
        && y > -0.108776*x-22.714517
        && y > 0.278158*x-79.223946
        && y < -0.387852*x+20.658908)
            continue;

        //if not in Australia, we give away this node.
        data.pop_back();
    }
}

void read_data_from_select_file()
{
    ifstream fin("Victoria.csv");
    string line;
    getline(fin,line);
    while(getline(fin,line))
        data.push_back(Node::getNode(line));
}

/*
 * this function is used to generate a csv file
 * which stores the data that all in Victoria.
 */

void write_file_after_select()
{
    ofstream fout;
    fout.open("Victoria.csv",ios::out);
    fout<<"latitude,longitude,brightness,date,time,fre"<<endl;
    for(int i = 0;i < data.size();i++)
    {
        fout<<data[i].latitude<<','<<data[i].longitude<<','<<data[i].brightness<<','<<data[i].date[0]\
        <<'-'<<data[i].date[1]<<'-'<<data[i].date[2]<<','<<data[i].times<<','<<data[i].fre<<endl;
    }
    fout.close();
}

void write_visual_data()
{
    ofstream fout;
    fout.open("graph.csv",ios::out);
    fout<<"latitude,longitude,brightness,Point Order,Polygon ID"<<endl;
    int index = 1;
    for(int i = 0;i < clusters.size();i++)
    {
        for(int j = 0;j < clusters[i].size();j++)
        {
            if(clusters[i][j].tag > 0 && Fires[i].area > 0.25)
            {
                fout << clusters[i][j].latitude << ',' << clusters[i][j].longitude << ','\
                << clusters[i][j].brightness << ',' << index++ << ',' << clusters[i][j].tag << endl;
            }
        }
    }
    fout.close();
}

void write_visual_data_with_scale()
{
    ofstream fout;
    fout.open("graph.csv",ios::out);
    fout<<"latitude,longitude,value,frequency,Point Order,Polygon ID"<<endl;
    int index = 1;
    for(int i = 0;i < clusters.size();i++)
    {
        for(int j = 0;j < clusters[i].size();j++)
        {
            if(clusters[i][j].tag > 0 && Fires[i].area > 0.25)
            {
                fout << clusters[i][j].latitude << ',' << clusters[i][j].longitude << ','\
                << Fires[i].value <<','<<Fires[i].frequency<< ',' << index++ << ',' << clusters[i][j].tag << endl;
            }
        }
    }
    fout.close();
}

void generate_fire()
{
    k = clusters.size();

    Fires.resize(k);
    for(int i = 0;i < k;i++)
    {
        Fires[i] = Fire(clusters[i]);
        Fires[i].area = Graham(clusters[i],i+1);
        Fires[i].value += Fires[i].area+Fires[i].nodes.size()/3000;
    }
    //sort(Fires.begin(),Fires.end(),Fire_cmp_area);
    /*
    double sum = 0;
    for(int i = 0;i < k;i++)
     sum += Fires[i].area;
    cout<<"sum = "<<sum<<endl;
     */

}

double calculate_sum_area()
{
    vector<Node> a(4);
    a[0].latitude = -34.009966;
    a[0].longitude = 140.952893;
    a[1].latitude = -38.047986;
    a[1].longitude = 140.963566;
    a[2].latitude = -38.600636;
    a[2].longitude = 146.044185;
    a[3].latitude = -37.508068;
    a[3].longitude = 149.972058;
    return Graham(a);
}

void delete_data()
{
    ifstream fin("graph.csv");;
    ofstream fout;
    string line;
    getline(fin,line);


    fout.open("new_graph.csv",ios::out);
    fout<<"latitude,longitude,value,frequency,Point Order,Polygon ID"<<endl;
    int index = 1;
    int tag = 0;
    int pre = -1;
    while(getline(fin,line))
    {
        string tmp = "";
        while(line.back() != ',')
        {
            tmp = line.back() + tmp;
            line.pop_back();
        }
        int ID = stoi(tmp);
        if(ID == 16 || ID == 38) continue;
        if(ID != pre) tag++;
        pre = ID;
        line += to_string(tag);
        fout<<line<<endl;
    }
    fout.close();
}

void Model_I()
{
    clusters = Model_I_part1(k,data);
    generate_fire();
    EOC_pos = add_EOC();
    Model_I_part2(Fires);
    merged_Fires = merge_Fires(EOC_pos,Fires);
    EOCS = Model_I_part3(merged_Fires,EOC_pos);
}