#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
/*================[Private defines]=======================*/
#define ROW_LENGTH 4
#define COL_LENGTH 4
#define DEBOUNCE_DELAY 20						//En ms
#define GPIO_ROW GPIOC
#define GPIO_COL GPIOC

/*================[Private data types]=====================*/
typedef enum{
	ROW_1 = GPIO_PIN_6,
	ROW_2 = GPIO_PIN_7,
	ROW_3 = GPIO_PIN_8,
	ROW_4 = GPIO_PIN_9
}row_t;

typedef enum{
	COL_1 = GPIO_PIN_0,
	COL_2 = GPIO_PIN_1,
	COL_3 = GPIO_PIN_2,
	COL_4 = GPIO_PIN_3
}col_t;

typedef enum{
	IDLE_COL = 0,
	ACTIVE_COL
}col_state;

/*================[Private variables]=====================*/
row_t keypad_rows[ROW_LENGTH] = {ROW_1, ROW_2, ROW_3, ROW_4};
col_t keypad_cols[COL_LENGTH] = {COL_1, COL_2, COL_3, COL_4};
char keypad_chars[ROW_LENGTH][COL_LENGTH] = { {'1', '2', '3', 'A'},
	      	  	  	  	  		  	  	  	  {'4', '5', '6', 'B'},
											  {'7', '8', '9', 'C'},
											  {'*', '0', '#', 'D'} };

/*================[Private functions]====================*/
static void set_row(row_t row){
	HAL_GPIO_WritePin(GPIO_ROW, row, GPIO_PIN_SET);
	return;
}

static void reset_row(row_t row){
	HAL_GPIO_WritePin(GPIO_ROW, row, GPIO_PIN_RESET);
	return;
}

static col_state read_col(col_t col){
	col_state state;
	if(HAL_GPIO_ReadPin(GPIO_COL, col) == GPIO_PIN_SET){
		vTaskDelay( 20 /portTICK_RATE_MS);
		if(HAL_GPIO_ReadPin(GPIO_COL, col) == GPIO_PIN_SET){
			state = ACTIVE_COL;
		}
		else{
			state = IDLE_COL;
		}
	}
	else{
		state = IDLE_COL;
	}
	return state;
}

/*================[Public functions]=====================*/
uint8_t read_keypad(void){
	uint8_t key_pressed = 0;
	for(uint8_t i = 0; i < ROW_LENGTH; i++){
		set_row(keypad_rows[i]);
		for(uint8_t j = 0; j < COL_LENGTH; j++){
			if(read_col(keypad_cols[j]) == ACTIVE_COL){
				key_pressed = (uint8_t) keypad_chars[i][j];
				reset_row(keypad_rows[i]);
				vTaskDelay( 100 /portTICK_RATE_MS);
				return key_pressed;
			}
		}
		reset_row(keypad_rows[i]);
	}
	return key_pressed;
}
