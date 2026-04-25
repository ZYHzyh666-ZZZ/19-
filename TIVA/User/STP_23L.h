#ifndef _STP_23L_H_
#define _STP_23L_H_

#include "sysconfig.h"

typedef struct {
	int16_t distance;  						/* 距离数据：测量目标距离单位 mm */
	uint16_t noise;		 						/* 环境噪声：当前测量环境下的外部环境噪声，越大说明噪声越大 */
	uint32_t peak;								/* 接收强度信息：测量目标反射回的光强度 */
	uint8_t confidence;						/* 置信度：由环境噪声和接收强度信息融合后的测量点的可信度 */
	uint32_t intg;     						/* 积分次数：当前传感器测量的积分次数 */
	int16_t reftof;   						/* 温度表征值：测量芯片内部温度变化表征值，只是一个温度变化量无法与真实温度对应 */
}LidarPointTypedef;

extern LidarPointTypedef Pack_Data[12];/* 雷达接收的数据储存在这个变量之中 */
extern LidarPointTypedef Pack_sum;     /* 12次输出结果储存 */

/*			12次平均存放变量			*/
extern int16_t STP23L_distance;			//距离
extern int16_t noise;				//噪声
extern int16_t reftof;				//可靠率
extern uint16_t peak,intg;


void STP23L_DataAnl(void);

void STP23L_get_onebyte(u8 data);
void STP23L_data_process(void);



#endif
