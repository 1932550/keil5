#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "CountSensor.h" 
uint16_t CountSensor_Count=0;			//全局变量，用于计数
//uint16_t CountSensor_Cou=0;
uint8_t KeyNum;		//定义用于接收按键键码的变量
uint8_t KeyNuma;
uint8_t KeyNumb;
uint8_t KeyNumc;
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	LED_Init();		//LED初始化
	Key_Init();		//按键初始化
	CountSensor_Init();

	while (1){
		GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11);
	OLED_ShowString(1, 1, "red");
	OLED_ShowString(2, 1, "yellow");
	OLED_ShowString(3, 1, "green");
	while (1){if(KeyNum != 4){
		KeyNum = Key_GetNum();		//获取按键键码
		
		if (KeyNum == 1)			//按键1按下
		{
			LED1_Turn();			//LED1翻转
		}
		
		if (KeyNum == 2)			//按键2按下
		{
			LED2_Turn();			//LED2翻转
		}
		if (KeyNum == 3)			//按键2按下
		{
			LED3_Turn();			//LED2翻转
		}
	}else{break;}
	 }   OLED_Clear();		                                                                          //ok
	
	OLED_ShowString(1, 1, "red");
	OLED_ShowString(2, 1, "yellow");
	OLED_ShowString(3, 1, "green");
	                                                   
	while (1)
	{
		GPIO_Write(GPIOB, ~0x0002);
		KeyNuma=1;KeyNumb=0;KeyNumc=0;  OLED_ShowString(1, 8, "ON ");//0000 0000 0000 0001，PA1引脚为低电平，其他引脚均为高电平，注意数据有按位取反
		Delay_s(1);				OLED_ShowString(1, 8, "OFF");
		
		GPIO_Write(GPIOB, ~0x0400);
		KeyNuma=0;KeyNumb=1;KeyNumc=0;	OLED_ShowString(2,8, "ON ");//0000 0000 0000 0010，PA引脚为低电平，其他引脚均为高电平
		Delay_s(1);				OLED_ShowString(2, 8, "OFF");
		
		GPIO_Write(GPIOB, ~0x0800);
		KeyNuma=0;KeyNumb=0;KeyNumc=1;  OLED_ShowString(3,8, "ON ");//0000 0000 0000 0100，PA引脚为低电平，其他引脚均为高电平
		Delay_s(1);             OLED_ShowString(3, 8, "OFF");
		
		}
	OLED_Clear();//有始有终
	}
	

}
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) == SET)		//判断是否是外部中断14号线触发的中断
	{//KeyNum=0;
		//如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
    {  
	while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
	{
		
		CountSensor_Count++;	
		Delay_ms(100);	
	}
	
	if(CountSensor_Count>15){
	    OLED_Clear(); 
		OLED_ShowString(1, 1, "Pause");//Delay_s(1);
		CountSensor_Count=0;
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1){}  //暂停
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){
		CountSensor_Count++;	
		Delay_ms(100);
		} 
		while(CountSensor_Count<15){CountSensor_Count=0;
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){CountSensor_Count++;
				Delay_ms(100);
			}
		
		}
		OLED_Clear(); 
		OLED_ShowString(1, 1, "running");
		Delay_s(1);
		OLED_Clear();
		OLED_ShowString(1,1,"red");
		OLED_ShowString(2, 1, "yellow");
	    OLED_ShowString(3, 1, "green");
		if(KeyNuma==1){OLED_ShowString(1, 8, "ON");OLED_ShowString(2, 8, "OFF");OLED_ShowString(3, 8, "OFF");
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);}
		if(KeyNumb==1){OLED_ShowString(1, 8, "OFF");OLED_ShowString(2, 8, "ON");OLED_ShowString(3, 8, "OFF");
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);}
		if(KeyNumc==1){OLED_ShowString(1, 8, "OFF");OLED_ShowString(2, 8, "OFF");OLED_ShowString(3, 8, "ON");
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);}
	Delay_s(1);
     while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0){}
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1){}CountSensor_Count=0;OLED_Clear(); 
	}else{KeyNum=4; CountSensor_Count=0;
		GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11);
		OLED_Clear();
	}
	//计数值自增一次
}
		EXTI_ClearITPendingBit(EXTI_Line6);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
}


	