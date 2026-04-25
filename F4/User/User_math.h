#ifndef __USER_MATH_H_
#define __USER_MATH_H_

#include "SysConfig.h"


double cos_f(double x);     //计算x的余弦值，x为弧度值
double sin_f(double x);     //计算x的正弦值，x为弧度值
double tan_f(double x);     //计算x的正切值，x为弧度值
double atan_f(double x);    //计算x的反正切值，返回值为弧度值
double sqrt_f(double x);    //计算x的平方根值


double change_angle_range(double angle);//将角度限制在-180到180度之间
double Angle_calculation(double angle, double target_angle);//计算最短夹角

#endif
