#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "stm32f4xx_conf.h"


void RCC_Configuration(void);
void TIM_Configuration(void);
void GPIO_Configuration(void);


void demo1(void);
void demo2(void);

static uint16_t pulse = 42;    

int main(void)
{
 
demo2();
	
}   
  
  
void demo1(void)
{ 	RCC_Configuration();
  	TIM_Configuration();
  	GPIO_Configuration();
	
	 int n=1;
	 int brightness=0;
	
	while(1)
	{	
		 if (((brightness + n) >= 1680) || ((brightness + n) <= 0))
   		 n = -n; // if  brightness maximum/maximum change direction
 		
		brightness += n;

  		TIM4->CCR1 = brightness - 1; // set brightness
		
		int i;
	   	for(i=0;i<200000;i++);  // delay about 1.6M instruction ->0.0086sec
	}

}


void demo2(void)
{	
	volatile int i; 
  	 RCC_Configuration();
  	TIM_Configuration();
  	GPIO_Configuration();

  while(1)  // Do not exit
  {
	TIM4->CCR1=pulse; //ang0 //pulse=42
	
	for(i=0;i<30000000;i++);  // delay about 24M instruction ->1.3sec
	
	//TIM4->CCR1=pulse+84; //ang90
	TIM4->CCR1=pulse+168; //ang180
	
	for(i=0;i<30000000;i++);  // delay
  }
}

void RCC_Configuration(void)
{
   	RCC_AHB1PeriphClockCmd(  RCC_AHB1Periph_GPIOB , ENABLE ); //Enalbe AHB for GPIOB
   	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE ); //Enable APB for TIM4
}

/**
  * @brief  configure the PD12~15 to Timers
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure
 	
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // Alt Function - Push Pull
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOB, &GPIO_InitStructure );  

 }


void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // Let PWM frequency equal 100Hz.
    // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
    // Solving for prescaler gives 240.
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1680 - 1;   //84000000/1680*1000=50hz  20ms for cycle
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1; 
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
    TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStruct );
    
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    
    // Initial duty cycle equals 0%. Value can range from zero to 65535.
    //TIM_Pulse = TIM4_CCR1 register (16 bits)
    TIM_OCInitStruct.TIM_Pulse = 0; //(0=Always Off, 65535=Always On)
 
    TIM_OC1Init( TIM4, &TIM_OCInitStruct ); // Channel 1  LED
    
 
    TIM_Cmd( TIM4, ENABLE );
}


