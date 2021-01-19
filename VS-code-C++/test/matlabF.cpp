#include "matlabF.h"
#include <math.h>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

double* createNums(double startNum, double interal , double endNum)   //创建等间隔数组
{  
    //注意函数里使用形参指针对分配的空间赋值，这样返回真的的指针指的就是首地址；
    //注意指针的指向
    double *numLine;
    int count = (endNum - startNum) / interal + 1;
    numLine = new double [count];
    
    double *time = numLine;
    for (int i = 0; i < (endNum - startNum) / interal + 1; i++)
    {
        *numLine = startNum + interal * i;
        numLine++;

    }
    return time;
}

int lengthArray(double *numArray)                                 //得到指针数组的长度
{
    double *myArray = numArray;
    int length;

//    int length = _msize(numArray)/sizeof(numArray[0]);

    return length;
}

double *t_effLine(double *Time, double t_total, int length)        //得到效率-时间曲线
{
    double *myTime = Time;
    double *Yimt_total = new double[length];
    double *myYimt = Yimt_total;
    int flag = 0;
    for (int i = 0; i < length; i++)
    {   
        if(myTime[i]<t_total)
            flag = 1;
        else
            flag = 0;

        *myYimt = (-0.0007 * myTime[i] + 0.7268) * flag;
        myYimt++;
    }

    return Yimt_total;
}

double *t_hjLine(double *Time, double Time_erosion, int length)    //定义喉径-时间曲线
{
    double *myTime = Time;
    double *Dt = new double[length];
    double *myDt = Dt;
    int flag1 = 0;
    int flag2 = 0;
    for (int i = 0; i < length;i++)
    {
        if(myTime[i] < Time_erosion)
            flag1 = 1;
        else
            flag1 = 0;
            
        if(myTime[i] >= Time_erosion)
            flag2 = 1;
        else
            flag2 = 0;

        *myDt = 36 * flag1 + (36 + 0.09666 * ((myTime[i] * 2 - 1) * 0.5 - Time_erosion)) * flag2;
        myDt++;
    }

    return Dt;
}

int fileRow(char *fileName)                                       //读取文件的行数
{
    int Row = 0;
    string str_line;
    ifstream in_file(fileName);
    in_file.seekg(0, ios::beg);  //定位到文件开始。
    while(getline(in_file,str_line))
    {
        Row++;
    }

    return Row;
}

double* readData(char *fileName,int line ,int readCol,int length,int gSelec)
{
    double *array = new double[length];
    double *myArray = array;
    int colCount = 0;
    int i;
    double g = 9.813;  //重力加速度
    ifstream in_file(fileName);
    
	
	char buf[1024];
	in_file.seekg(0, ios::beg);  //定位到文件开始。
	for (i = 0; i < line; i++)
	{
		in_file.getline(buf, sizeof(buf));//读取行。
	}

    string str_line;
    int j = 0;
    while(getline(in_file, str_line)) //getline最后的符号不是很懂，忽略‘xx’符号？
    {   
        string str_tmp;
        stringstream ss(str_line);       //stringstream 输出自动以空格为分隔符

            while (ss >> str_tmp)
            {
                if(colCount == readCol)
                {   
                    if(gSelec)
                    {
                        array[j] = atof(str_tmp.c_str());
                        array[j] = g * array[j];
                        // cout << str_tmp << " " << endl;
                        j++;
                    }
                    else
                    {
                        array[j] = atof(str_tmp.c_str());
                        // cout << str_tmp << " " << endl;
                        j++;
                    }

                    break;
                }
                colCount++;
            }
        colCount = 0;
    }
    
    in_file.close();

    return myArray;
}

double *arrayMulti(double *a, double *b, int length, double k/*系数*/)
{
    double *result = new double[length];
    double *myResult = result;
    for (int i = 0; i < length; i++)
    {
        *result = k * a[i] * b[i];
        result++;
    }

    return myResult;
}

double *arrayDivi(double *a, double *b, int length, double k /*系数*/, int mode)
{
    double *result = new double[length];
    for (int i = 0; i < length; i++)
    {   
        if(mode == 0)
        {
            result[i] = k / b[i];
        }
        else if(mode == 1)
        {
            result[i] = k * a[i] / b[i];
        }
        
    }

    return result;
}

double *arrayZero(int length)
{
    double *myAarry = new double[length];
    for (int i = 0; i < length; i++)
    {
        myAarry[i] = 0;
    }
    return myAarry;
}

double *arrayNum(int length, double num)
{
    double *myAarry = new double[length];
    for (int i = 0; i < length; i++)
    {
        myAarry[i] = num;
    }
    return myAarry;
}

double interp1D(double *x, double *y, double length, double x_simple)
{
    int count = 0;
    double x1, x2, y1, y2;
    double k, b;
    double y_simple;
    while (x_simple < x[count] && count < length)
        count++;
    x1 = x[count];
    x2 = x[count + 1];
    y1 = y[count];
    y2 = y[count + 1];
    k = (y2 - y1) / (x2 - x1);
    b = y2 - k * x2;
    y_simple = k * x_simple + b;
    return y_simple;
}

double meanArray(double *array, int startPoint, int endPoint)
{
    double *myArray = array;
    double sum,mean;
    int i;
    sum = 0;
    for (i = startPoint - 1; i < (endPoint - 1); i++)
    {
        sum += myArray[i];
    }

    mean = sum / (endPoint - startPoint + 1);
    return mean;
}

double *qpExpress(double *array, int length)
{
    double* myArray = new double[length];
    double k = 0.15929203539;
    for (int i = 0; i < length; i++)
    {
        myArray[i] = k * array[i] + 4;
    }
    return myArray;
}


// void test(double* *out)                                                          //测试一下
// {
//     double *Time      =  createNums(0.5, 0.5, 400.0); //稳定工作时间
//     *out = Time;
    
// }