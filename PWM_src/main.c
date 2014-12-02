/*
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPEEXTI0_IRQHandlerCT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"

/** @addtogroup STM32F4-Discovery_PWM_Output_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void TIM_Configuration(void);
void GPIO_Configuration(void);
void Interrupts_Configuration(void);

void demo1(void);
void demo2(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

 static uint16_t pulse = 60;    




int main(void)
{
 
demo1();
	
 
  return(0); // System will implode
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
	   	for(i=0;i<200000;i++);  // delay
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
	TIM4->CCR1=pulse;
	
	for(i=0;i<30000000;i++);  // delay
	
	TIM4->CCR1=pulse+82;
	
	for(i=0;i<30000000;i++);  // delay
  }
}

void RCC_Configuration(void)
{
   	RCC_AHB1PeriphClockCmd(  RCC_AHB1Periph_GPIOB , ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE );
   	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );
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

/**
  * @brief  configure the TIM4 for PWM mode
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // Let PWM frequency equal 100Hz.
    // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
    // Solving for prescaler gives 240.
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1680 - 1;   //84000000/1680*1000=50hz  20ms for duty cycle
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 1; 
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
    TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStruct );
    
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    
    // Initial duty cycle equals 0%. Value can range from zero to 65535.
    //TIM_Pulse = TIM4_CCR1 register (16 bits)
    TIM_OCInitStruct.TIM_Pulse = 65535; //(0=Always Off, 65535=Always On)
 
    TIM_OC1Init( TIM4, &TIM_OCInitStruct ); // Channel 1  LED
    TIM_OC2Init( TIM4, &TIM_OCInitStruct ); // Channel 1  LED
   
 
    TIM_Cmd( TIM4, ENABLE );
}


