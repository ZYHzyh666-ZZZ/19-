#include "Drv_I2C.h"
#include "Drv_Uart.h"

/*--------------------------------------------------------------------------------------*/
/*                          I2C通信底层函数                                             */

///* 宏定义：SCL/SDA操作 */
//#define SCL_H()     GPIO_SetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN)
//#define SCL_L()     GPIO_ResetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN)

///* SDA输出高低 */
//#define SDA_H()     GPIO_SetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN)
//#define SDA_L()     GPIO_ResetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN)

///* 读取SDA引脚电平 */
//#define SDA_READ()  GPIO_ReadInputDataBit(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN)




/*协议层*/

/**
  * 函    数：I2C写SCL引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCL的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCL为低电平，当BitValue为1时，需要置SCL为高电平
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
	if(BitValue)//根据BitValue，设置SCL引脚的电平
		GPIO_SetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN);
	else
		GPIO_ResetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SCL_PIN);
//	Delay_us(10);												//延时10us，防止时序频率超过要求
}

/**
  * 函    数：I2C写SDA引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SDA的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SDA为低电平，当BitValue为1时，需要置SDA为高电平
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
	if(BitValue)//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
		GPIO_SetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN);
	else
		GPIO_ResetBits(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN);		
//	Delay_us(10);												//延时10us，防止时序频率超过要求
}

/**
  * 函    数：I2C读SDA引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前SDA的电平，范围0~1
  * 注意事项：此函数需要用户实现内容，当前SDA为低电平时，返回0，当前SDA为高电平时，返回1
  */
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SOFT_I2C_GPIO_PORT, SOFT_I2C_SDA_PIN);		//读取SDA电平
//	Delay_us(10);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}

/**
  * 函    数：I2C起始
  * 参    数：无
  * 返 回 值：无
  */
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);							//释放SDA，确保SDA为高电平
	MyI2C_W_SCL(1);							//释放SCL，确保SCL为高电平
	MyI2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
	MyI2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

/**
  * 函    数：I2C终止
  * 参    数：无
  * 返 回 值：无
  */
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
	MyI2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
	MyI2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
}

/**
  * 函    数：I2C发送一个字节
  * 参    数：Byte 要发送的一个字节数据，范围：0x00~0xFF
  * 返 回 值：无
  */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		/*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
		MyI2C_W_SDA(Byte & (0x80 >> i));//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		MyI2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		MyI2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}


/**
  * 函    数：I2C接收一个字节
  * 参    数：无
  * 返 回 值：接收到的一个字节数据，范围：0x00~0xFF
  */
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		MyI2C_W_SCL(1);						//释放SCL，主机机在SCL高电平期间读取SDA
		if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
													//当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		MyI2C_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

/**
  * 函    数：I2C发送应答位
  * 参    数：Byte 要发送的应答位，范围：0~1，0表示应答，1表示非应答
  * 返 回 值：无
  */
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);					//主机把应答位数据放到SDA线
	MyI2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

/**
  * 函    数：I2C接收应答位
  * 参    数：无
  * 返 回 值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
  */
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;							//定义应答位变量
	MyI2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	MyI2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = MyI2C_R_SDA();					//将应答位存储到变量里
	MyI2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}

uint16_t I2C_Read2Byte(u8 reg_addr)//读取
{
	u16 reg_data=0;
	u16 temp=0;
	MyI2C_Start();
	MyI2C_SendByte(WRITE_ADDR);
	if(MyI2C_ReceiveAck())return 0;
	MyI2C_SendByte(reg_addr);   
	if(MyI2C_ReceiveAck())return 0;
	MyI2C_Start();
	MyI2C_SendByte(READ_ADDR);
	if(MyI2C_ReceiveAck())return 0;
	reg_data= MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	reg_data=(reg_data<<8)&0xFF00;
	temp=MyI2C_ReceiveByte();
	MyI2C_SendAck(0);
	MyI2C_Stop();
	reg_data|=temp;
	return reg_data;
}

uint8_t I2C_Write2Byte(u8 reg_addr,u16 reg_data)//写入
{        
	u8 data_high=(u8)((reg_data&0xFF00)>>8);
	u8 data_low=(u8)reg_data&0x00FF;
	MyI2C_Start();
	MyI2C_SendByte(WRITE_ADDR);   
	if(MyI2C_ReceiveAck())return 0;
	MyI2C_SendByte(reg_addr);    
	if(MyI2C_ReceiveAck())return 0;        
	MyI2C_SendByte(data_high);
	if(MyI2C_ReceiveAck())return 0;        
	MyI2C_SendByte(data_low);
	if(MyI2C_ReceiveAck())return 0;                 
	MyI2C_Stop();
//	delay_ms(2);
	return 1;
}


/* 简单延时函数（需根据系统时钟调整）*/
void SoftI2C_DelayUs(uint32_t us)
{
    /* 如果已有微秒级延时函数，直接调用；否则可以用循环粗略延时 */
    /* 示例：使用SysTick实现的精确延时，或者简单循环 */
    uint32_t count = us * 42;  /* 粗略估算，168MHz主频时约42个循环/微秒，需实际测试 */
    while (count--) {
        __NOP();
    }
}

/*--------------------------------------------------------------------------------------*/
/*                                          I2C应用层函数                                */


/**
  * 函数作用：  I2C初始化
  * 参数1：     无
  * 返回值：    无
  * 
  * 备注：      无
  * 更新日期：   2026-3-17
  */
void Drv_I2C_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(SOFT_I2C_GPIO_CLK, ENABLE);
	
    /* 先配置SCL和SDA为开漏输出，高电平（空闲状态）*/
    GPIO_InitStruct.GPIO_Pin = SOFT_I2C_SCL_PIN | SOFT_I2C_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_I2C_GPIO_PORT, &GPIO_InitStruct);

    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
    SoftI2C_DelayUs(5);
	
	I2C_Write2Byte(Config_Reg,0x4527);//0100_010_100_100_111 //16次平均,1.1ms,1.1ms,连续测量分流电压和总线电压
	I2C_Write2Byte(Calib_Reg,0x06AB);//40A最大,0.005欧
}


// /**
//   * 函数作用：  I2C写入
//   * 参数1：     无
//   * 返回值：    无
//   * 
//   * 备注：      无
//   * 更新日期：   2026-3-17
//   */
// void I2C_Test_Write(void)
// {
//     uint8_t data = 0xAA;
//     I2C_Master_Transmit(I2C1, 0x50, &data, 1);
// }

/* 简单示例：从从机地址0x50读取一个字节 */
void INA226_Read(uint16_t* data)
{
	float Bus_data, Pow_data, Current_data;
    Bus_data = (float)I2C_Read2Byte(Bus_V_Reg);
	Bus_data *= 1.25;
	Bus_data /= 1000;
	fc_bat.st_data.voltage_100 = (uint16_t)(Bus_data * 100);
	
	Pow_data = (float)I2C_Read2Byte(Power_Reg);
	Pow_data *= 25;
	Pow_data *= 3;
	
	Current_data = (float)I2C_Read2Byte(Current_Reg);
	Current_data *= 3;
	
//	Printf_uart2("%fV\t%fA\t%fW\n",Bus_data,Current_data/1000,Pow_data/1000);
}

