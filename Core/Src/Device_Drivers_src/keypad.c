#include "keypad.h"
/*Funciones relacionadas al keypad*/
eventoDisplay HandleInput(FSM *FSM1){
	eventoDisplay eventoEnviado;
	uint8_t KeyPressed;
	if((xTaskGetTickCount() - FSM1->StartTime) < pdMS_TO_TICKS(KEYPAD_TIMEOUT)){		//Mientras no se supere el timeout puedo ingresar digito
		KeyPressed = GetKey();
		if(KeyPressed != 0){
			if(KeyPressed == BORRAR_DIGITO){
				FSM1->CurrentIndex--;
				FSM1->Buffer[FSM1->CurrentIndex] = 0;					//El digito obtenido en el estado anterior lo borro
				FSM1->StartTime = xTaskGetTickCount();					//Tomo nueva referencia para calcular timeout
				FSM1->CurrentState--;									//Vuelvo al estado anterior
				eventoEnviado = PANTALLA_BORRAR_ENTRADA;
			}
			else{
				if(KeyPressed == ENTER){
					/*Esta seccion maneja el caso de secuencia incompleta*/
					FSM1->CurrentState = INCOMPLETO;
					eventoEnviado = PANTALLA_SECUENCIA_INCOMPLETA;
				}
				else{
					/*Esta seccion se encarga de tomar cualquier otro digito*/
					FSM1->Buffer[FSM1->CurrentIndex] = KeyPressed;			//Guardo el digito en el buffer
					FSM1->CurrentIndex++;
					FSM1->StartTime = xTaskGetTickCount();
					FSM1->CurrentState++;									//Paso al siguiente estado
					eventoEnviado = PANTALLA_AÑADIR_ENTRADA;
				}

			}
		}
		else{
			eventoEnviado = PANTALLA_IDLE;
		}
	}
	else{
		FSM1->CurrentState = TIMEOUT;
		eventoEnviado = PANTALLA_TIMEOUT;
	}
	return eventoEnviado;
}

uint8_t GetKey(void){
	/*Esta funcion realiza un barrido al keypad matricial 4x4*/
	/*Si se presiona una tecla, devuelve el ascii asociado. Si no, devuelve 0*/
	uint16_t PinRows[ROWS] = {GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9};
	uint16_t PinCols[COLS] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
	uint8_t KeyPressed;
	char Keypad[ROWS][COLS] = {
	      {'1', '2', '3', 'A'},
	      {'4', '5', '6', 'B'},
	      {'7', '8', '9', 'C'},
	      {'*', '0', '#', 'D'}
	 };
	 /*Escaneo el teclado en busca de 1 tecla presionada. Si hay tecla presionada*/
	 for(uint8_t i = 0; i < ROWS; i++){
		 HAL_GPIO_WritePin(GPIOC, PinRows[i], GPIO_PIN_SET);					//Prendo y apago secuencialmente las filas
		 for(uint8_t j = 0; j < COLS; j++){
			 if(HAL_GPIO_ReadPin(GPIOC, PinCols[j]) == GPIO_PIN_SET){			//Barro las columnas y verifico si se presiona una tecla
				 vTaskDelay(20 / portTICK_RATE_MS);								//Espero 20 ms y reconfirmo el estado. Si no se reconfirma sigue el barrido.
				 if(HAL_GPIO_ReadPin(GPIOC, PinCols[j]) == GPIO_PIN_SET){
					 KeyPressed = (uint8_t) Keypad[i][j];
					 HAL_GPIO_WritePin(GPIOC, PinRows[i], GPIO_PIN_RESET);		//Pongo fila a 0 antes de salir de la funcion.
					 vTaskDelay(150/ portTICK_RATE_MS);
					 return KeyPressed;
				 }
			 }
		 }
		 HAL_GPIO_WritePin(GPIOC, PinRows[i], GPIO_PIN_RESET);
	 }
	 return 0;
}

uint8_t AreVectorsEqual(uint8_t *Vector1, uint8_t *Vector2, uint8_t size){
	/*Compara si 2 vectores son iguales elemento a elemento. Devuelve 1 si lo son, 0 si no lo son.*/
	uint8_t Pass = 1;
	for(uint8_t i = 0; i < size; i++){
		if(Vector1[i] != Vector2[i]){
			Pass = 0;
			i = size;
		}
	}
	return Pass;
}

void ClearString(char *str){
	/*Esta funcion limpia el string utilizado para mostrar mensajes por UART*/
	while(*str != 0){
		*str = 0;
		str++;
	}
	return;
}

void ClearBuffer(uint8_t *Buffer, uint8_t size){
	/*Limpia el buffer que guarda la secuencia ingresada*/
	for(uint8_t i = 0; i < size; i++){
		Buffer[i] = 0;
	}
	return;
}
