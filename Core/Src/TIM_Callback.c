
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

#define Out_XL				0xA800
#define Out_XH				0xA900
#define Out_YL				0xAA00
#define Out_YH				0xAB00

extern TIM_HandleTypeDef htim5;
extern int TIM_State;
extern SPI_HandleTypeDef hspi1;
extern int SPI_State;
extern uint16_t	RX;
extern uint16_t	TX;

void LCD_Set_Data(uint8_t _data, uint8_t _rs);

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{
	if (htim == &htim5)
	{
		switch(TIM_State)
		{
			case 1:
			{	
				TIM_State++;
				LCD_Set_Data(0x3C, 0);
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
				LL_TIM_SetAutoReload(TIM5, 2000);
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 2:
			{	
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
				TIM_State++;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 3:
			{	
				TIM_State++;
				LCD_Set_Data(0x0F, 0);
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
				LL_TIM_SetAutoReload(TIM5, 2000);
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 4:
			{	
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
				TIM_State++;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 5:
			{	
				TIM_State++;
				LCD_Set_Data(0x01, 0);
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
				LL_TIM_SetAutoReload(TIM5, 2000);
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 6:
			{	
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
				TIM_State++;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 7:
			{	
				TIM_State++;
				LCD_Set_Data(0x06, 0);
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
				LL_TIM_SetAutoReload(TIM5, 2000);
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 8:
			{	
				HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
				TIM_State++;
				HAL_TIM_Base_Start_IT(&htim5);
				break;
			}
			case 9:
			{
				SPI_State++;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);
				TIM_State++;
				break;
				//HAL_TIM_Base_Start_IT(&htim5);
			}	
			case 10:
			{
				//RX = 0x0000;
				uint16_t TX = Out_XL;
				SPI_State++;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);
				TIM_State++;
				break;
				//HAL_TIM_Base_Start_IT(&htim5);
			}
			case 11:
			{
				//RX = 0x0000;
				uint16_t TX = Out_XH;
				SPI_State++;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);
				TIM_State++;
				break;
				//HAL_TIM_Base_Start_IT(&htim5);
			}
			case 12:
			{
				//RX = 0x0000;
				uint16_t TX = Out_YL;
				SPI_State++;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);
				TIM_State++;
				break;
				//HAL_TIM_Base_Start_IT(&htim5);
			}
			case 13:
			{
				//RX = 0x0000;
				uint16_t TX = Out_YH;
				SPI_State++;
				HAL_SPI_TransmitReceive_IT(&hspi1, (uint8_t *)&TX, (uint8_t *)&RX, 1);
				TIM_State++;
				break;
				//HAL_TIM_Base_Start_IT(&htim5);
			}			
			default:
			{
				TIM_State = 0;
				break;
			}
		}
	}
}

void LCD_Set_Data(uint8_t _data, uint8_t _rs)
{
	if(_rs == 0)
	{
		HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
	}
	uint32_t tmp = LL_GPIO_ReadOutputPort(GPIOD);
	tmp &= 0xFF00;
	tmp |= _data;
	LL_GPIO_WriteOutputPort(GPIOD, tmp);
}
