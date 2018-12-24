//���
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

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

//������
 int main(void)
 {		
    int key;
	u8 wifi_con=0;//��¼wifi����״̬ 1:���� 0:�Ͽ�  
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
 
