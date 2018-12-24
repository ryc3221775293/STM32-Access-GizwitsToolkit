#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "adc.h"
#include "lsens.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 
/************************************************  
 ALIENTEK精英STM32开发板     
 作者：唯恋殊雨    
 CSDN博客：https://blog.csdn.net/tichimi3375    
 OUT-PF8 
************************************************/   
/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}


 int main(void)
 {	 
 	u8 adcx; 
	int key;
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	Lsens_Init(); 			//初始化光敏传感器   
	Gizwits_Init();         //协议初始化
	printf("--------机智云IOT-DHT11温湿度检测报警实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");	 
	while(1)
	{
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con?printf("connect"):printf("close");
		 }
		 
		 
		adcx=Lsens_Get_Val();
		printf("光照强度为：%d",adcx);//显示ADC的值 
		currentDataPoint.valueLight =  adcx;
		if(currentDataPoint.valueLED == 0x01)
		{
				LED0 = 0;
		}else
		{
				LED0 = 1;
		}
		 
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		
		key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
			wifi_sta=0;//标志wifi已断开
		}
		delay_ms(200);
	}
}
 
