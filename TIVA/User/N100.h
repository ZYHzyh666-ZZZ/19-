#ifndef _N100_H_
#define _N100_H_

#include "sysconfig.h"

enum
{
	cmd_IMU = 0x40,
	cmd_AHRS = 0x41,
	cmd_INS = 0x42,
	cmd_RAW =0x58,
};

typedef struct
{
	double V_Roll;
	double V_Pitch;
	double V_Yaw;

	double Roll;
	double Pitch;
	double Yaw;
	
	double Q1;
	double Q2;
	double Q3;
	double Q4;

}N100_AHRS_def;

typedef struct
{
	double Velocity_X;
	double Velocity_Y;
	double Velocity_Z;

	double Acceleration_X;
	double Acceleration_Y;
	double Acceleration_Z;
	
	double Location_X;
	double Location_Y;
	double Location_Z;
	
	double Pressure_Altitude;

}N100_INS_def;

typedef struct
{
	N100_AHRS_def AHRS;
	N100_INS_def INS;
	
}N100_def;

extern N100_def MY_N100;


void N100_DataAnl(u8* data, int data_len);
void N100_get_onebyte(u8 data);

float Data_Trans(u8 Data_1,u8 Data_2,u8 Data_3,u8 Data_4);

#endif
