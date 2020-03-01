#ifndef __display_h
#define __display_h

#include "stm32f4xx_hal.h"
#include "main.h"

typedef enum DISP_letter_label{
	DISP_A = 0,
	DISP_B = 1,
	DISP_C = 2,
	DISP_D = 3,
	DISP_F = 4,
	DISP_G = 5,
	DISP_H = 6,
	DISP_DOT = 7
}DISP_letter_label;

typedef	enum DISP_dig_label{
		DIG_1 = 0,
		DIG_2 = 1,
		DIG_3 = 2,
		DIG_4 = 3
} DISP_dig_label;

extern int tmpNumbsTable[4];
extern int tmpDotsTable[4];
extern int numbersTable[10];

extern GPIO_TypeDef *dispPorts[]; 
extern int dispPins[];
extern GPIO_TypeDef *digsPorts[];
extern int digsPins[];

void DISP_WriteDispPin(DISP_letter_label label, GPIO_PinState state);
void DISP_WriteDigPin(DISP_dig_label label, GPIO_PinState state);
void DISP_WriteDisplayByte(int number, int dot);
void DISP_Init(void);
void DISP_SetValue(char NumbsTable[4], char DotsTable[4]);
void DISP_SetValueFloat(double number, int precision);
void DISP_Update(void);
#endif
