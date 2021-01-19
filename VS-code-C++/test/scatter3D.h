#ifndef SCATTER3D_H
#define SCATTER3D_H


class scatter3D
{
private:
    /* data */
    //定义用到的一些常量
    int MAX_DATASET;
    int MAX_DIMENSION;
    double GRAVITY;
    double UPPERBOUND_X;
    double LOWERBOUND_X;
    double STEP_X;
    double UPPERBOUND_Y;
    double LOWERBOUND_Y;
    double STEP_Y;
    double UPPERBOUND_Z;
    double LOWERBOUND_Z;
    double STEP_Z;

    double EPS;

    double *close_x;
    double *close_y;
    double *close_z;
    double *close_v;

public:
    scatter3D(/* args */int length);
    ~scatter3D();

    double scatteredInterpolant(double x, double y, double z /* args */);                //读取数据
    void find8LimitPoint(double pre_x, double pre_y, double pre_z);                      //找上下界8个点
    double findLowerBound(double x, double step, double lowerBound, double upperBound);  //找数值的位置

    //放在private里面是无法被访问的，只有类自己内部可以访问
    double *point_x;
    double *point_y;
    double *point_z;
    double *point_v;
};


#endif