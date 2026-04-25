#include "User_math.h"

double cos_f(double x)
{
    double term = 1; // 第一个项
    double sum = 1;  // 初始化和
    int n = 1;

    while (term > 0.000001 || term < -0.000001) 
    {
        term = -term * x * x / (2 * n * (2 * n - 1));
        sum += term;
        n++;
    }
    return sum;
}

double sin_f(double x) 
{
    double term = x; // 第一个项
    double sum = x;  // 初始化和
    int n = 1;

    while (term > 0.000001 || term < -0.000001) 
    {
        term = -term * x * x / (2 * n * (2 * n + 1));
        sum += term;
        n++;
    }
    return sum;
}

double tan_f(double x)
{
    return sin_f(x) / cos_f(x);
}

double atan_f(double x)
{
    double term = x; // 第一个项
    double sum = x;  // 初始化和
    int n = 1;

    while (term > 0.000001 || term < -0.000001) 
    {
        term = -term * x * x * (2 * n - 1) / (2 * n + 1);
        sum += term;
        n++;
    }
    return sum;
}

double sqrt_f(double x)
{
    if (x < 0) return -1; // 错误输入
    if (x == 0) return 0;

    double guess = x / 2.0; // 初始猜测值
    double epsilon = 0.000001; // 精度要求

    while (1) 
    {
        double next_guess = (guess + x / guess) / 2.0;
        if (next_guess - guess < epsilon && next_guess - guess > -epsilon) 
        {
            break;
        }
        guess = next_guess;
    }
    return guess;
}

double change_angle_range(double angle)//将角度限制在-180到180度之间
{
    while (angle > 180.0)
        angle -= 360.0;
    while (angle < -180.0)
        angle += 360.0;
    return angle;
}

double Angle_calculation(double angle, double target_angle)//计算最短夹角
{
    double angle_error;
    angle_error = target_angle - angle;
    angle_error = change_angle_range(angle_error);
    return angle_error;
}
