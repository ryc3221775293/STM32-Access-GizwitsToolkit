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
 ALIENTEK��ӢSTM32������     
 ���ߣ�Ψ������    
 CSDN���ͣ�https://blog.csdn.net/tichimi3375    
 OUT-PF8 
************************************************/   
/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;

//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0;

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}


 int main(void)
 {	 
 	u8 adcx; 
	int key;
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	Lsens_Init(); 			//��ʼ������������   
	Gizwits_Init();         //Э���ʼ��
	printf("--------������IOT-DHT11��ʪ�ȼ�ⱨ��ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");	 
	while(1)
	{
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con?printf("connect"):printf("close");
		 }
		 
		 
		adcx=Lsens_Get_Val();
		printf("����ǿ��Ϊ��%d",adcx);//��ʾADC��ֵ 
		currentDataPoint.valueLight =  adcx;
		if(currentDataPoint.valueLED == 0x01)
		{
				LED0 = 0;
		}else
		{
				LED0 = 1;
		}
		 
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		
		key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ 
			wifi_sta=0;//��־wifi�ѶϿ�
		}
		delay_ms(200);
	}
}
 
