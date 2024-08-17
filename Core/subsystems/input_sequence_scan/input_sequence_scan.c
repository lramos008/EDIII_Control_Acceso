#include "main.h"
#include "cmsis_os.h"
#include "keypad_functions.h"
#include "display_functions.h"
#include <stdbool.h>
/*================[Extern variables]======================*/
extern QueueHandle_t display_queue;
extern QueueHandle_t sequence_queue;

/*================[Private defines]=======================*/
#define SEQUENCE_LENGTH 6
#define BORRAR_DIGITO '#'
#define ENTER '*'
#define KEYPAD_TIMEOUT 5000
/*================[Public data types]=====================*/
char buffer[SEQUENCE_LENGTH + 1];
typedef enum{
	ESPERANDO_DIGITO_1,
	ESPERANDO_DIGITO_2,
	ESPERANDO_DIGITO_3,
	ESPERANDO_DIGITO_4,
	ESPERANDO_DIGITO_5,
	ESPERANDO_DIGITO_6,
	ESPERANDO_CONFIRMACION,
	PROCEDER_A_BUSQUEDA_DE_USUARIO,
	SECUENCIA_INCOMPLETA,
	TIMEOUT
}keypad_state_t;
/*================[Private functions]====================*/
static void clear_buffer(char *buffer, uint8_t size){
	/*Limpia el buffer que guarda la secuencia ingresada*/
	for(uint8_t i = 0; i < size; i++){
		buffer[i] = 0;
	}
	return;
}

static void send_sequence(char *sequence, uint8_t sequence_length){
	for(uint8_t i = 0; i < sequence_length; i++){
		xQueueSend(sequence_queue, &sequence[i], portMAX_DELAY);
	}
	return;
}

static display_state_t handle_keypad_input(uint8_t input){
	/*Declaro variables a utilizar*/
	display_state_t current_screen;
	static keypad_state_t current_state = ESPERANDO_DIGITO_1;
	//static char buffer[SEQUENCE_LENGTH + 1];
	static uint8_t current_index;
	static TickType_t start_time;
	/*Comienzo de la maquina de estados*/
	switch(current_state){
	case ESPERANDO_DIGITO_1:
		if(input != 0){
			if((input != BORRAR_DIGITO) && (input != ENTER)){
				current_index = 0;
				buffer[current_index] = input;
				current_index++;
				start_time = xTaskGetTickCount();
				current_state = ESPERANDO_DIGITO_2;
				current_screen = PANTALLA_AGREGAR_ENTRADA;
			}
		}
		else{
			current_screen = PANTALLA_DE_INICIO;
		}
		break;
	case ESPERANDO_DIGITO_2:
	case ESPERANDO_DIGITO_3:
	case ESPERANDO_DIGITO_4:
	case ESPERANDO_DIGITO_5:
	case ESPERANDO_DIGITO_6:
	case ESPERANDO_CONFIRMACION:
		if((xTaskGetTickCount() - start_time) < pdMS_TO_TICKS(KEYPAD_TIMEOUT)){
			if(input != 0){
				if(input == BORRAR_DIGITO){
					current_index--;
					buffer[current_index] = 0;
					start_time = xTaskGetTickCount();
					current_state--;
					current_screen = PANTALLA_BORRAR_ENTRADA;
				}
				else{
					if(input == ENTER){
						if(current_state == ESPERANDO_CONFIRMACION){
							current_state = PROCEDER_A_BUSQUEDA_DE_USUARIO;
							current_screen = PANTALLA_BUSQUEDA_DE_USUARIO;
						}
						else{
							current_state = SECUENCIA_INCOMPLETA;
							current_screen = PANTALLA_SECUENCIA_INCOMPLETA;
						}
					}
					else{
						if(current_state != ESPERANDO_CONFIRMACION){
							buffer[current_index] = input;
							current_index++;
							start_time = xTaskGetTickCount();
							current_state++;
							current_screen = PANTALLA_AGREGAR_ENTRADA;
						}
						else{
							current_screen = PANTALLA_IDLE;
						}
					}
				}
			}
			else{
				current_screen = PANTALLA_IDLE;
			}
		}
		else{
			current_state = TIMEOUT;
			current_screen = PANTALLA_TIMEOUT;
		}
		break;
	case PROCEDER_A_BUSQUEDA_DE_USUARIO:
		send_sequence(buffer, SEQUENCE_LENGTH);
		clear_buffer(buffer, SEQUENCE_LENGTH);
		current_state = ESPERANDO_DIGITO_1;
		current_screen = PANTALLA_DE_INICIO;
		break;
	case SECUENCIA_INCOMPLETA:
	case TIMEOUT:
	default:
		clear_buffer(buffer, SEQUENCE_LENGTH);
		current_state = ESPERANDO_DIGITO_1;
		current_screen = PANTALLA_DE_INICIO;
		break;
	}
	/*Enviar pantalla al display*/
	return current_screen;
}

/*================[Public Task]=====================*/
void input_sequence_scan(void *pvParameters){
	display_state_t current_screen = PANTALLA_DE_INICIO;
	uint8_t input;
	/*Pongo la pantalla inicial*/
	send_screen_to_display(current_screen);
	while(1){
		input = read_keypad();
		current_screen = handle_keypad_input(input);
		send_screen_to_display(current_screen);
	}
}


