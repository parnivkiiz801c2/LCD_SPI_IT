#include "stdlib.h"
#include "stdio.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

#define Out_XL				0x28
#define Out_XH				0x29
#define Out_YL				0x2A
#define Out_YH				0x2B
#define angle_step		183.33

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim5;
extern int SPI_State;
extern uint16_t	RX;
extern uint16_t Out_X;
extern uint16_t Y_Out;
extern int Int_DataX;
extern int Int_DataY;
extern float angle_X, Y_angle;
extern char DataX[20];
extern char DataY[20];

void HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * hspi)
{
	if(hspi == &hspi1)
	{
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
		switch(SPI_State)
		{
			case 1: 
			{
				if ((RX & 0xFF) == 0x3F)
				{
					SPI_State++;
					HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
					uint16_t TX = 0x2063;
					HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);					
				}
				break;
			}
			case 2:
			{
				SPI_State++;
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
				uint16_t TX = 0xA000;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);									
				break;
			}
			case 3:
			{
				if ((RX & 0xFF) == 0x63)
				{
					//Start smth
					LL_TIM_SetAutoReload(TIM5, 15000);
					HAL_TIM_Base_Start_IT(&htim5);
				}
				break;
			}
			case 4:
			{
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
				Out_X	= RX << 8;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 5:
			{
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
				Out_X	= Out_X + RX;
				if ((Out_X & 0x8000) != 0) Int_DataX = (-1 - (0xFFFF - Out_X));
				else Int_DataX = Out_X;
				angle_X = Int_DataX / angle_step;
				sprintf(DataX, "%.2f", angle_X);
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 6:
			{
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
				Y_Out	= RX << 8;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 7:
			{
				HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
				Y_Out	= Y_Out + RX;
				if ((Y_Out & 0x8000) != 0) Int_DataY = (-1 - (0xFFFF - Y_Out));
				else Int_DataY = Y_Out;
				Y_angle = Int_DataY / angle_step;
				sprintf(DataY, "%.2f", Y_angle);
				SPI_State = 3;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			default:
			{
				SPI_State = 0;
				break;
			}
		}
	}
}
