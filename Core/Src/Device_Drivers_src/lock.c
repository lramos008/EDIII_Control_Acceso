#include "lock.h"

void openLock(void){
	/*Wrapper de la función writepin*/
	HAL_GPIO_WritePin(Lock_GPIO_Port, Lock_Pin, GPIO_PIN_RESET);
	return;
}

void closeLock(void){
	/*Wrapper de la función writepin*/
	HAL_GPIO_WritePin(Lock_GPIO_Port, Lock_Pin, GPIO_PIN_SET);
	return;
}
