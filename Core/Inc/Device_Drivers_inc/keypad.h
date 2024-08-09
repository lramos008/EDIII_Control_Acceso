#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H
/*Agrego includes necesarios*/
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include "screen.h"
/*Incluyo defines*/
#define ROWS 4
#define COLS 4
#define DEBOUNCE_DELAY 20						//En ms
#define KEYPAD_TIMEOUT 5000 					//En ms
#define BORRAR_DIGITO '#'
#define ENTER '*'
/*Defino enums*/
typedef enum{
	ESPERA_DIGITO_1 = 0,
	ESPERA_DIGITO_2,
	ESPERA_DIGITO_3,
	ESPERA_DIGITO_4,
	ESPERA_DIGITO_5,
	ESPERA_DIGITO_6,
	ESPERA_ENTER,
	TIMEOUT,
	INCOMPLETO,
}KeypadState;

/*Defino estructura para guardar parametros de la maquina de estado*/
typedef struct{
  	uint8_t 	Buffer[6];
	uint8_t 	CurrentIndex;
	KeypadState CurrentState;
	TickType_t 	StartTime;
}FSM;

/*Agrego prototipos de funciones*/
uint8_t GetKey(void);
uint8_t AreVectorsEqual(uint8_t *Vector1, uint8_t *Vector2, uint8_t size);
eventoDisplay HandleInput(FSM *FSM1);
void ClearString(char *str);
void ClearBuffer(uint8_t *Buffer, uint8_t size);
#endif /* KEYPAD_DRIVER_H */
