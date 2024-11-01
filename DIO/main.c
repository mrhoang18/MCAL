#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#include "Dio.h"

void GPIO_Config(void);

void Dio_ToggleChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType currentLevel = Dio_ReadChannel(ChannelId); 

    if (currentLevel == STD_HIGH)
    {
        Dio_WriteChannel(ChannelId, STD_LOW); 
    }
    else
    {
        Dio_WriteChannel(ChannelId, STD_HIGH);
    }
}

void Delay_Us(uint32_t Time)
{
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < Time);
}

void Delay_Ms(uint32_t Time)
{
    while (Time > 0) 
    {
        Delay_Us(1000); 
        Time--;        
    }
}

// Configure TIM2 to use as a microsecond delay timer
void TIM2_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // Prescale to 1 MHz (1 µs period)
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;    // Maximum period for counting
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

int main(void)
{
    GPIO_Config();
	TIM2_Config();
	
	
	// volatile Dio_PortLevelType portLevel;

	/********* Test Dio_ReadChannelGroup(&channelGroup). Continue in while(1) ************

	// Define a channel group to read pins PA5 PA6 PA7
	Dio_ChannelGroupType channelGroup = {
		.port = 0,           // 0 corresponds to GPIOA
		.offset = 5,         // Start from pin 5 (PA5)
		.mask = 0x00E0       // Mask 0b 0000 0000 1110 0000 to select pins PA5 PA6 PA
	};

	// Variable to store the result
	volatile Dio_PortLevelType groupLevel;
	***************************************************************************************
	*/
	
	
	/********* Test Dio_ReadChannelGroup(&channelGroup). Continue in while(1) ************
	
	// Define a channel group to read pins PA5 PA6 PA7
	Dio_ChannelGroupType channelGroup = {
		.port = 0,           // 0 corresponds to GPIOA
		.offset = 5,         // Start from pin 5 (PA5)
		.mask = 0x00E0       // Mask 0b 0000 0000 1110 0000 to select pins PA5 PA6 PA
	};
	***************************************************************************************
	*/

    while (1)
    {	
				
		// portLevel = Dio_ReadPort(2);

		/********** Test Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level) **********
		
		Dio_WritePort(0, 0x0000);	// 16 bit = PA0->PA15
		Delay_Ms(500);
		Dio_WritePort(0, 0xFFFF);	// 16 bit = PA0->PA15
		Delay_Ms(500);
		
		* Debug result:  PA0->PA15 = STD_HIGH<->STD_LOW (blink led)
		***************************************************************************************
		*/
		
			
		/********** Test Dio_ReadChannelGroup(&channelGroup) *********************************
		
		Dio_ToggleChannel(5);
		Dio_WriteChannel(6, STD_LOW);
		Dio_ToggleChannel(7);
		groupLevel = Dio_ReadChannelGroup(&channelGroup);
		Delay_Ms(500);
		
		* Debug result:  groupLevel = 0x0A<->0x00
		***************************************************************************************
		*/
		
		
		/********** Test Dio_WriteChannelGroup(&channelGroup,...) *****************************
		
		Dio_WriteChannelGroup(&channelGroup, 0x05);	// 
		Delay_Ms(500);
		Dio_WriteChannelGroup(&channelGroup, 0x00);	//
		Delay_Ms(500);
		
		* Debug result: 0x05 = 0b101, meaning: PA5, PA7 are High, PA6 is LOW.
						0x00 = 0b000, meaning: PA5, PA6, PA7 are LOW.
		***************************************************************************************
		*/
		
//		 // Toggle (flip) the state of the specified channel
//       Dio_LevelType newLevel = Dio_FlipChannel(5);
//		 Delay_Ms(500);

		Dio_ToggleChannel(7);
		Delay_Ms(500);
		
    }
}

void GPIO_Config(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

