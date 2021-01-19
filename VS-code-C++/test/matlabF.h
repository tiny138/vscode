// #pragma once
#ifndef MATLABF_H
#define MATLABF_H

    double *createNums(double startNum, double interal, double endNum);              //创建等间隔数组
    int lengthArray(double *numArray);                                               //得到指针数组的长度
    double *t_effLine(double *Time, double t_total, int length);                     //得到效率-时间曲线
    double *t_hjLine(double *Time, double Time_erosion, int length);                 //定义喉径-时间曲线
    int fileRow(char *fileName);                                                     //读取文件的行数
    double *readData(char *fileName, int line, int readCol, int length, int gSelec); //从第line行开始读数据，读第readCol列
    double *arrayMulti(double *a, double *b, int length, double k);                  //矩阵乘法，值存储在新的指针中
    double *arrayDivi(double *a, double *b, int length, double k, int mode);         //矩阵除法，值存储在新的指针中
    double *arrayZero(int length);                                                   //创建长度length均为0的数组
    double *arrayNum(int length, double num);                                        //创建长度length均为x的数组
    double interp1D(double *x, double *y, double length, double x_simple);           //得到x_simple的插值
    double meanArray(double *array, int startPoint, int endPoint);                   //求平均值
    double *qpExpress(double *array, int length);                                    //气瓶压力表达式
    void test(double **out);                                                          //测试一下

#endif