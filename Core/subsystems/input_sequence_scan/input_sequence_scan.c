#include "main.h"
#include "cmsis_os.h"
#include "keypad_functions.h"
#include <stdbool.h>
/*================[Private defines]=======================*/
#define SEQUENCE_LENGTH 6
#define BORRAR_DIGITO '#'
#define ENTER '*'
#define KEYPAD_TIMEOUT 5000
/*================[Public data types]=====================*/
typedef enum{
	ESPERANDO_DIGITO_1,
	ESPERANDO_DIGITO_2,
	ESPERANDO_DIGITO_3,
	ESPERANDO_DIGITO_4,
	ESPERANDO_DIGITO_5,
	ESPERANDO_DIGITO_6,
	ESPERANDO_CONFIRMACION,
	PROCEDER_A_RECONOCIMIENTO,
	SECUENCIA_INCOMPLETA,
	TIMEOUT
}keypad_state_t;

/*================[Private functions]====================*/
void clear_buffer(uint8_t *buffer, uint8_t size){
	/*Limpia el buffer que guarda la secuencia ingresada*/
	for(uint8_t i = 0; i < size; i++){
		buffer[i] = 0;
	}
	return;
}

static keypad_state_t handle_keypad_input(uint8_t input){
	static uint8_t current_index;
	static uint8_t buffer[SEQUENCE_LENGTH];
	static keypad_state_t current_state = ESPERANDO_DIGITO_1;
	static TickType_t start_time;
	switch(current_state){
	case ESPERANDO_DIGITO_1:
	case ESPERANDO_DIGITO_2:
	case ESPERANDO_DIGITO_3:
	case ESPERANDO_DIGITO_4:
	case ESPERANDO_DIGITO_5:
	case ESPERANDO_DIGITO_6:
	case ESPERANDO_CONFIRMACION:
		if(current_state == ESPERANDO_DIGITO_1){
			if((input != BORRAR_DIGITO) && (input != ENTER)){
				current_index = 0;
				buffer[current_index] = input;
				current_index++;
				start_time = xTaskGetTickCount();
				current_state = ESPERANDO_DIGITO_2;
			}
		}
		else{
			if((xTaskGetTickCount() - start_time) < pdMS_TO_TICKS(KEYPAD_TIMEOUT)){
				if(input == BORRAR_DIGITO){
					current_index--;
					buffer[current_index] = 0;
					start_time = xTaskGetTickCount();
					current_state--;
				}
				else{
					if(input == ENTER){
						if(current_state == ESPERANDO_CONFIRMACION){
							current_state = CHEQUEANDO_EN_BASE_DE_DATOS;
						}
						else{
							current_state = SECUENCIA_INCOMPLETA;
						}
					}
					else{
						if(current_state != ESPERANDO_CONFIRMACION){
							buffer[current_index] = input;
							current_index++;
							start_time = xTaskGetTickCount();
							current_state++;
						}
					}

				}
			}
			else{
				current_state = TIMEOUT;
			}
		}
		break;
	case PROCEDER_A_RECONOCIMIENTO:
		break;
	case SECUENCIA_INCOMPLETA:
	case TIMEOUT:
	default:
		clear_buffer(buffer, SEQUENCE_LENGTH);
		current_state = ESPERANDO_DIGITO_1;
		break;
	}
	return current_state;
}

static void send_event_to_display(keypad_state_t current_state){
	static keypad_state_t last_state = ESPERANDO_DIGITO_1;
	if(current_state == last_state){

	}
	switch(current_state){
	case ESPERANDO_DIGITO_1:
		/*Pantalla de inicio*/
		break;
	case ESPERANDO_DIGITO_2:
	case ESPERANDO_DIGITO_3:
	case ESPERANDO_DIGITO_4:
	case ESPERANDO_DIGITO_5:
	case ESPERANDO_DIGITO_6:
	case ESPERANDO_CONFIRMACION:
		if(current_state > last_state){
			/*Agregar entrada*/
		}
		else{
			if(current_state < last_state){
				/*Sacar entrada*/
			}
			else{
				/*Enviar estado idle al display*/
			}
		}
		break;
	case PROCEDER_A_RECONOCIMIENTO:
		break;
	case SECUENCIA_INCOMPLETA:
		break;
	case TIMEOUT:
		break;
	default:
		/*Estar idle por default*/
		break;
	}
}

/*================[Public functions]=====================*/
void input_sequence_scan(void *pvParameters){
	static keypad_state_t current_state;
	uint8_t input;
	while(1){
		input = read_keypad();
		if(input != 0){
			current_state = handle_keypad_input(input);
		}
		send_event_to_display(current_state);
	}
}


