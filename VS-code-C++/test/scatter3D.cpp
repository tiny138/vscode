#include "scatter3D.h"
#include <iostream>
using namespace std;

scatter3D::scatter3D(/* args */int length)
{
    //定义用到的一些常量
    MAX_DATASET = length;
    MAX_DIMENSION = 8;
    GRAVITY = 9.813;
    UPPERBOUND_X = 10;
    LOWERBOUND_X = 2;
    STEP_X = 0.5;
    UPPERBOUND_Y = 3;
    LOWERBOUND_Y = 0;
    STEP_Y = 0.2;
    UPPERBOUND_Z = 12;
    LOWERBOUND_Z = 4;
    STEP_Z = 0.5;
    EPS = 1e-6;

    close_x = new double[MAX_DIMENSION];
    close_y = new double[MAX_DIMENSION];
    close_z = new double[MAX_DIMENSION];
    close_v = new double[MAX_DIMENSION];
}

scatter3D::~scatter3D()
{
}

double scatter3D::scatteredInterpolant(double x, double y, double z)
{
	find8LimitPoint(x, y, z);
	//find8ClosestPoint(dataset, closest, pre, dataRows);

	double xd = (x - findLowerBound(x, STEP_X, LOWERBOUND_X, UPPERBOUND_X)) / STEP_X;
	double yd = (y - findLowerBound(y, STEP_Y, LOWERBOUND_Y, UPPERBOUND_Y)) / STEP_Y;
	double zd = (z - findLowerBound(z, STEP_Z, LOWERBOUND_Z, UPPERBOUND_Z)) / STEP_Z;

	double c00 = close_v[0] * (1 - xd) + close_v[4] * xd;
	double c01 = close_v[1] * (1 - xd) + close_v[5] * xd;
	double c10 = close_v[2] * (1 - xd) + close_v[6] * xd;
	double c11 = close_v[3] * (1 - xd) + close_v[7] * xd;

	double c0 = c00 * (1 - yd) + c10 * yd;
	double c1 = c01 * (1 - yd) + c11 * yd;

	double pre_v = c0 * (1 - zd) + c1 * zd;
	return pre_v;

}

// x的步长为0.5，y的步长为0.2，z的步长为0.5
void scatter3D::find8LimitPoint(double pre_x, double pre_y, double pre_z)
{

	double x = pre_x;
	double y = pre_y;
	double z = pre_z;

	double xLower = findLowerBound(x, STEP_X, LOWERBOUND_X, UPPERBOUND_X);
	double xUpper = xLower + STEP_X;
	double yLower = findLowerBound(y, STEP_Y, LOWERBOUND_Y, UPPERBOUND_Y);
	double yUpper = yLower + STEP_Y;
	double zLower = findLowerBound(z, STEP_Z, LOWERBOUND_Z, UPPERBOUND_Z);
	double zUpper = zLower + STEP_Z;

	// 找到对应的八个点按顺序放入res中,顺序为000,001,010...,111
	close_x[0] = xLower;
	close_y[0] = yLower;
	close_z[0] = zLower;

	close_x[1] = xLower;
	close_y[1] = yLower;
	close_z[1] = zUpper;

	close_x[2] = xLower;
	close_y[2] = yUpper;
	close_z[2] = zLower;

	close_x[3] = xLower;
	close_y[3] = yUpper;
	close_z[3] = zUpper;

	close_x[4] = xUpper;
	close_y[4] = yLower;
	close_z[4] = zLower;

	close_x[5] = xUpper;
	close_y[5] = yLower;
	close_z[5] = zUpper;

	close_x[6] = xUpper;
	close_y[6] = yUpper;
	close_z[6] = zLower;

	close_x[7] = xUpper;
	close_y[7] = yUpper;
	close_z[7] = zUpper;

	// 根据xyz查找在dataset中对应的点，可以优化为折半查找等高效率数组查找方式
	for (int i = 0; i < MAX_DIMENSION; i++) {
		for (int j = 0; j < MAX_DATASET; j++) {
			if (abs(close_x[i] - point_x[j]) < EPS
				&& abs(close_y[i] - point_y[j]) < EPS
				&& abs(close_z[i] - point_z[j]) < EPS)
            {
                close_v[i] = point_v[j];
                break;
			}
		}
	}

}

double scatter3D::findLowerBound(double x, double step, double lowerBound, double upperBound) {
    if (x > upperBound || x < lowerBound)
    { 	
		if (abs(x - upperBound) > 0.01 && abs(x - lowerBound) > 0.01)
        cout<<"预测点数值越界"<<endl;
    }
	return lowerBound + step * int(((x - lowerBound) / step));
}