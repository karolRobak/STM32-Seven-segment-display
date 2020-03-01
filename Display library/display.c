#include "display.h"

	int tmpNumbsTable[4];
	int tmpDotsTable[4];
	int numbersTable[10] = {0x3f, 0x06,0x5b, 0x4f, 0x66, 0x6d, 0x7d,0x07,0x7f,0x6f};
	
	
	GPIO_TypeDef *dispPorts[] = {
		// Change these values according to your STM32 configuration
		GPIOA, // DISP_A
		GPIOB, // DISP_B
		GPIOB, // DISP_C
		GPIOB, // DISP_D
		GPIOB, // DISP_F
		GPIOA, // DISP_G
		GPIOB, // DISP_H
		DISP_DOT_GPIO_Port // DISP_DOT
	}; 
	
	int dispPins[] = {
		// Change these values according to your STM32 configuration
		DISP_A_Pin,
		DISP_B_Pin,
		DISP_C_Pin,
		DISP_D_Pin,
		DISP_E_Pin,
		DISP_F_Pin,
		DISP_G_Pin,
		DISP_DOT_Pin
	};
	
	GPIO_TypeDef *digsPorts[] = {
		// Change these values according to your STM32 configuration
		GPIOA, 
		GPIOA, 
		DIG_3_GPIO_Port,
		DIG_4_GPIO_Port		
	};
	
	int digsPins[] = {
		// Change these values according to your STM32 configuration
		 DIG_1_Pin, 
		 DIG_2_Pin, 
		 DIG_3_Pin,
		 DIG_4_Pin
	};
	
	void DISP_WriteDispPin(DISP_letter_label label, GPIO_PinState state){
		HAL_GPIO_WritePin(dispPorts[label],dispPins[label],state);
	}
	
	void DISP_WriteDigPin(DISP_dig_label label, GPIO_PinState state){
		HAL_GPIO_WritePin(digsPorts[label],digsPins[label],state);
	}
	
	
	void DISP_WriteDisplayByte(int number, int dot){
		int selectedNumber;
		if(number>=0 && number <=9){
			selectedNumber = numbersTable[number];
		}
		else{
			selectedNumber = 0;
		}
		
		for(DISP_letter_label i = DISP_A; i<DISP_DOT ;++i){
			if(selectedNumber>>i & 0x01){
				DISP_WriteDispPin(i, GPIO_PIN_RESET);
			}
			else{
				DISP_WriteDispPin(i,GPIO_PIN_SET);
			}
		}
		
		if(dot){
			DISP_WriteDispPin(DISP_DOT, GPIO_PIN_RESET);
		}
		else{
			DISP_WriteDispPin(DISP_DOT, GPIO_PIN_SET);
		}
	}

	
	void DISP_Init(void){
		for(DISP_dig_label i=DIG_1;i<=DIG_4;++i){
			tmpNumbsTable[i] = 0;
			tmpDotsTable[i] = 0;
		}
	}
	
	void DISP_SetValue(char NumbsTable[4], char DotsTable[4]){
		DISP_Init();
		for(DISP_dig_label i = DIG_1; i<=DIG_4; i++){
			// conversion char -> int
			tmpNumbsTable[i] = NumbsTable[i] - '0';
			
			if(DotsTable[i] == '.'){
				tmpDotsTable[i] = 1;
			}
			else{
				tmpDotsTable[i] = 0;
			}
		}
	}
	
	void DISP_SetValueFloat(double number, int precision){
		int tmpVal;
		DISP_Init();
		switch(precision){
			case 0: tmpVal = number; break;
			case 1: tmpVal = number * 10.0;	break;
			case 2:	tmpVal = number * 100.0; break;
			case 3: tmpVal = number * 1000.0; break;
			default: tmpVal = number; break;
		}
		
		tmpDotsTable[3 - precision] = 1; 
		tmpNumbsTable[3] = tmpVal % 10; 
		tmpNumbsTable[2] = (tmpVal % 100 - tmpNumbsTable[3])/10;
		tmpNumbsTable[1] = (tmpVal % 1000 - tmpNumbsTable[3] - tmpNumbsTable[2])/100;
		tmpNumbsTable[0] = (tmpVal % 10000 - tmpNumbsTable[3] - tmpNumbsTable[2] - tmpNumbsTable[1])/1000;
	}
	
	
	void DISP_Update(void){
		for(DISP_dig_label i = DIG_1; i<=DIG_4;++i){
			for(DISP_dig_label j = DIG_1; j<=DIG_4;++j){
				DISP_WriteDigPin(j, GPIO_PIN_RESET);
			}
			DISP_WriteDigPin(i, GPIO_PIN_SET);
			DISP_WriteDisplayByte(tmpNumbsTable[i],tmpDotsTable[i]);
			HAL_Delay(1);
		}	
	}
	
	