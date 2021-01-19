#pragma execution_character_set("GBK")
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "malloc.h"
#include "time.h"

#include "matlabF.h"
#include "scatter3D.h"
#include <iostream>
using namespace std;

int _tmain( int argc, _TCHAR *argv[] )
{  
    ///////////////////////////基本参数阶段开始////////////////////////////////////////
    double pi         = 3.1416;

    double m1         = 1.1;
    double t1         = 180;
    double m2         = 0.7;
    double Pa         = 0.101325;                     //环境大气压Mpa
    double r_ini      = 0.0915;                       //药柱初始直径m
    double L_grain    = 0.949;                        //药柱长度m
    double Ro_grain   = 1209;                         //药柱密度kg/s
    // mo_dot         = 1.1;                          //氧化剂流率kg/s
    double mf_ini     = 58.4;                         //药柱初始质量kg
    double mo_ini     = 226;                          //氧化剂初始质量kg
    double Yimt_Pc    = 0.937;                        //给定的燃烧效率
    // Yimt_total     = 0.86;                         //给定的总效率
    double Dt_ini     = 36.0;                         //初始喉部直径 
    double Erosion    = 0.0936;                       //烧蚀系数
    double De_ini     = 116;                          //初始出口直径
    double t2         = (mo_ini-m1*t1)/m2;
    double t_total    = t1+t2;
    double *Time      =  createNums(0, 0.5, 300.0); //稳定工作时间
    double *Time_eff  =  createNums(0,0.5,t_total);

    //转成C++ 就不转置了    
    // Time_eff             = Time_eff';
    // Time                 = Time';
    double Time_erosion     = 125 * 1.0 / m1;                          //开始烧蚀时间
    double Time_biantuili   = t1;                                      //变推力时间
    double Time_deta        = 0.5;                                     //时间间隔
    // int Npoint_eff          = lengthArray(Time_eff);                   //有效数据点数
    // int Npoint              = lengthArray(Time);                       //总数据点数
    int Npoint_eff          =  (t_total - 0) / 0.5 + 1;                   //有效数据点数
    int Npoint              = (300.0 - 0) / 0.5 + 1;                       //总数据点数
    //Npoint_ss             = 2*Time_erosion-1;  //开始烧蚀数据标号
    double Npoint_biantuili = 2*Time_biantuili+1;                      //开始变推力数据标号

    double *Yimt_total      = t_effLine(Time, t_total, Npoint);        //定义效率-时间曲线
    double *Dt              = t_hjLine(Time, Time_erosion, Npoint);    //定义喉径-时间曲线
    double Pc_ini           = 2.0;                                     //初始计算用的压强Mpa  
    double k                = 1.14;                                    //燃气比热比

    int fRow;                                                          //txt文件的总行数
    int fileBeg             = 8;                                       //跳过表头，从第fileBeg行开始读
    char *fileName          = "BD-20210111.txt";
    fRow                    = fileRow(fileName);

    
    //构建3维插值的类
    scatter3D FIsopt = scatter3D(fRow - fileBeg - 1);
    scatter3D FIsvac = scatter3D(fRow - fileBeg - 1);
    scatter3D FCstar = scatter3D(fRow - fileBeg - 1);

    //减1因为文件最后一行没用不读
    //读取3维的值
    //gSelec： 0:不乘g；1：乘g
    FIsopt.point_x = readData(fileName, fileBeg, 0, fRow - fileBeg - 1, 0);
    FIsopt.point_y = readData(fileName, fileBeg, 1, fRow - fileBeg - 1, 0); 
    FIsopt.point_z = readData(fileName, fileBeg, 3, fRow - fileBeg - 1, 0); 
    FIsopt.point_v = readData(fileName, fileBeg, 10, fRow - fileBeg - 1, 1);

    FIsvac.point_x = readData(fileName, fileBeg, 0, fRow - fileBeg - 1, 0);
    FIsvac.point_y = readData(fileName, fileBeg, 1, fRow - fileBeg - 1, 0);
    FIsvac.point_z = readData(fileName, fileBeg, 3, fRow - fileBeg - 1, 0);
    FIsvac.point_v = readData(fileName, fileBeg, 11, fRow - fileBeg - 1, 1);

    FCstar.point_x = readData(fileName, fileBeg, 0, fRow - fileBeg - 1, 0);
    FCstar.point_y = readData(fileName, fileBeg, 1, fRow - fileBeg - 1, 0);
    FCstar.point_z = readData(fileName, fileBeg, 3, fRow - fileBeg - 1, 0);
    FCstar.point_v = readData(fileName, fileBeg, 9, fRow - fileBeg - 1, 0);

    double *At    = arrayMulti(Dt, Dt, Npoint, pi / 4);
    double Ae_ini = pi * De_ini * De_ini / 4;
    double *AeAt  = arrayDivi(At, At, Npoint, Ae_ini, 0);
    ///////////////////////////基本参数阶段结束////////////////////////////////////////

    ///////////////////////////燃烧公式参数/////////////////////////////////////////// 
    ////r_dot=a*Go^n
    ////单位m
    //a_of_r_dot=4.621*10^-6;
    //n_of_r_dot=1.338;
    //20140606次试验拟合数据
    double a_of_r_dot = 3.2e-6;
    double n_of_r_dot = 1.3995;
    ///////////////////////二阶段燃速公式//////////////////////////////////////////////
    // a2_of_r_dot=6.85*10^-6;
    // n2_of_r_dot=1.0959;

    ////////////////////////////////定义变量////////////////////////////////////////
    double *r=arrayZero(Npoint);
    double *Go=arrayZero(Npoint);
    double *r_dot=arrayZero(Npoint);
    double *mf_dot=arrayZero(Npoint);
    double *mtotal_dot=arrayZero(Npoint);
    double *OF=arrayZero(Npoint);
    double *Cstar=arrayZero(Npoint);
    double *Isp=arrayZero(Npoint);
    double *Ivac=arrayZero(Npoint);
    double *Pc_th=arrayZero(Npoint);
    double *Pc_iter=arrayZero(4);
    double *mfremain=arrayZero(Npoint);
    double *moremain=arrayZero(Npoint);
    //////////////////////////////////end////////////////////////////////////////


    //////////////////////////////迭代求解初始Pc////////////////////////////////////
    //////相当于解耦Pc=(mf+mo)*Cstar/At与Cstar=f(OF,Pc)
    //////实际迭代表明：迭代2次即可收敛，这里给定4次
    int count;
    for (count = 0; count < 4; count++)
    {

        if(count == 0)
            Pc_iter[count] = Pc_ini;
        else
            Pc_iter[count] = Pc_th[0];
        r[0] = r_ini;
        Go[0] = m1 / (pi * r[0] * r[0]);
        r_dot[0] = a_of_r_dot * pow(Go[0], n_of_r_dot);
        mf_dot[0] = 2 * pi * r[0] * r_dot[0] * L_grain * Ro_grain;
        mfremain[0] = mf_ini;
        moremain[0] = mo_ini;
        mtotal_dot[0] = mf_dot[0] + m1;
        OF[0] = m1 / mf_dot[0];
        Cstar[0] = FCstar.scatteredInterpolant(OF[0],Pc_iter[count],AeAt[0]);
        Isp[0] = FIsopt.scatteredInterpolant(OF[0], Pc_iter[count], AeAt[0]);
        Pc_th[0] = mtotal_dot[0] * Cstar[0] / At[0];

    }
    //////////////////////////////////end////////////////////////////////////////

    ///////////////////////////////工作过程pc求解/////////////////////////////////
    for (count = 1; count < Npoint_biantuili; count++)
    {
        r[count] = r[count - 1] + r_dot[count - 1] * Time_deta;
        Go[count] = m1 / (pi * r[count] * r[count]);
        r_dot[count] = a_of_r_dot * pow(Go[count], n_of_r_dot);
        mf_dot[count] = 2 * pi * r[count] * r_dot[count] * L_grain * Ro_grain;
        mfremain[count] = mfremain[count - 1] - mf_dot[count - 1] * Time_deta;
        moremain[count] = moremain[count - 1] - m1 * Time_deta;
        mtotal_dot[count] = mf_dot[count] + m1;
        OF[count] = m1 / mf_dot[count];
        Cstar[count] = FCstar.scatteredInterpolant(OF[count],Pc_th[count-1],AeAt[count]);
        Isp[count] = FIsopt.scatteredInterpolant(OF[count], Pc_th[count-1], AeAt[count]);
        Pc_th[count] = mtotal_dot[count] * Cstar[count] / At[count];
    }

    for (count = Npoint_biantuili - 1; count < Npoint_eff; count++)
    {
        r[count] = r[count - 1] + r_dot[count - 1] * Time_deta;
        Go[count] = m2 / (pi * r[count] * r[count]);
        r_dot[count] = a_of_r_dot * pow(Go[count], n_of_r_dot);
        mf_dot[count] = 2 * pi * r[count] * r_dot[count] * L_grain * Ro_grain;
        mfremain[count] = mfremain[count - 1] - mf_dot[count - 1] * Time_deta;
        moremain[count] = moremain[count - 1] - m2 * Time_deta;
        mtotal_dot[count] = mf_dot[count] + m2;
        OF[count] = m2 / mf_dot[count];
        Cstar[count] = FCstar.scatteredInterpolant(OF[count],Pc_th[count-1],AeAt[count]);
        Isp[count] = FIsopt.scatteredInterpolant(OF[count], Pc_th[count-1], AeAt[count]);
        Pc_th[count] = mtotal_dot[count] * Cstar[count] / At[count];
    }

    for (count = Npoint_eff - 1; count < Npoint; count++)
    {
        r[count] = 0;
        Go[count] = 0;
        r_dot[count] = 0;
        mf_dot[count] = 0;
        mfremain[count] = 0;  //剩余燃料质量
        moremain[count] = 0;  //剩余氧化剂质量
        mtotal_dot[count] = 0;
        OF[count] = 0;
        Cstar[count] = 0;
        Isp[count] = 0;
        Pc_th[count] = 0;
    }


    ///////////////////////////////地面推力（Isg)求解/////////////////////////////////
    //Ft_ground_th=(mtotal_dot.*Ivac-Pa.*Ae_ini)/1000;     ////单位kN
    double *Ft_optimal_th = arrayMulti(mtotal_dot, Isp, Npoint, 0.001);
    double *Ft_vacum_th = arrayMulti(mtotal_dot, Ivac, Npoint, 0.001);
    ////////////////////////////////////end//////////////////////////////////////////  
    // load Pc_th_nihe.txt
    // Pc_th_nihe2(:,1)=Pc_th_nihe(:,2:2);
    // for i=170:Npoint
    //     Pc_th(i,1)=Pc_th_nihe2;
    // end
    double *Yimt_PcArray = arrayNum(Npoint, Yimt_Pc);
    double *Pc_cal = arrayMulti(Pc_th, Yimt_PcArray, Npoint, 1);                       //地面压力
    double *Ft_ground_cal = arrayMulti(Ft_optimal_th, Yimt_total, Npoint, 1000);       //地面推力

    ////////////////////////////////////计算出口压力//////////////////////////////////////////
    double *pepc = createNums(0.001, 0.0001, 0.1);
    int myLength = (0.1 - 0.001) / 0.0001 + 1;
    double *AeAt3 = new double[myLength];
    for (count = 0; count < myLength; count++)
    {
        AeAt3[count] = sqrt(k) * pow((2 / (k + 1)), (k + 1) / (2 * (k - 1))) / pow(pepc[count], (1 / k)) / sqrt(2 * k / (k - 1) * (1 - pow(pepc[count], (k - 1) / k)));
    }


    double pepc_1 = interp1D(AeAt3, pepc, myLength, 10.33);           //计算一阶段的膨胀比
    double pepc_2 = interp1D(AeAt3, pepc, myLength, 7.96);            //计算二阶段的膨胀比

    double Pc_1 = meanArray(Pc_cal,1,Npoint_biantuili);             ////计算一阶段的压力平均值
    double Pc_2 = meanArray(Pc_cal, Npoint_biantuili, Npoint_eff);  ////计算二阶段的压力平均值

    //计算气瓶压力
    double *Pgas = qpExpress(moremain, Npoint); //气瓶压力

    for (int cc = 0; cc < 601;cc++)
        cout << Ft_ground_cal[cc]<<endl;

        return 0;

}


// #pragma execution_character_set("GBK")
// #include "stdafx.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <math.h>
// #include "malloc.h"
// #include "time.h"

// #include "matlabF.h"
// #include "scatter3D.h"
// #include <iostream>
// #include<string>
// #include <ctime>
// using namespace std;

// double ** test1(double *in1, double *in2, int length);
// int _tmain( int argc, _TCHAR *argv[] )
// {  
//     ///////////////////////////基本参数阶段开始////////////////////////////////////////
//     double *IN1 = createNums(0.5, 0.5, 400.0);
//     int len = 800;
//     double *IN2 = arrayNum(len, 2.0);
//     double **out = test1(IN1, IN2, len);

//     cout << out[0][100] << endl;
//     cout<< out[1][100] << endl;

//        time_t now = time(0);
 
//    cout << "1970 到目前经过秒数:" << now << endl;
 
//    tm *ltm = localtime(&now);
 
//    // 输出 tm 结构的各个组成部分
//    cout << "年: "<< 1900 + ltm->tm_year << endl;
//    cout << "年: "<<  ltm->tm_year << endl;
//    cout << "月: "<< 1 + ltm->tm_mon<< endl;
//    cout << "月: "<< ltm->tm_mon<< endl;
//    cout << "日: "<<  ltm->tm_mday << endl;
//    cout << "时间: "<< ltm->tm_hour << ":";
//    cout << ltm->tm_min << ":";
//    cout << ltm->tm_sec << endl;
//    int i = 2;
//    char mychar[50];
//    sprintf_s(mychar,"%02d",(1 + ltm->tm_mon));
//    cout << mychar;
//    return 0;
// }


// double **test1(double *in1,double *in2,int length)                                                          //测试一下
// {
//     int i, j;
//     double** out = new double*[2];
//     for (i = 0; i < 2; i++)
//     {
//         out[i] = new double[length];
//     }
//         for (i = 0; i < 2; i++)
//         {
//             for (j = 0; j < length; j++)
//             {
//                 if (i == 0)
//                 {
//                     out[i][j] = in1[j];
//                 }
//                 else
//                 {
//                     out[i][j] = in2[j];
//                 }
//             }
//         }
//     return out;
// }