#include "main.h"
#include "cmsis_os.h"
#include "sd_functions.h"
#include "display_functions.h"
#include "signal_processing_functions.h"
#include <string.h>
#include <stdbool.h>
#define ARM_MATH_CM4
#include "arm_math.h"

/*================[Private defines]========================*/
#define SEQUENCE_LENGTH 6
#define BUFFER_SIZE 16384
#define NUMBER_OF_CHUNKS 8
#define CHUNK_SIZE (BUFFER_SIZE / NUMBER_OF_CHUNKS)
#define ADC_RESOLUTION 4096
/*================[Private variables]======================*/
display_state_t current_screen;
/*================[Extern variables]=======================*/
extern QueueHandle_t sequence_queue;
/*================[Private functions]======================*/
static void check_if_database_exists(char *name){
	bool file_exists;
	file_exists = check_if_file_exists(name);
	if(file_exists == false){
		send_uart("No existe la base de datos!!!\n\n");
		while(1){
			/*Reiniciar y cargar la base de datos*/
		}
	}
	return;
}

static void check_if_register_exists(char *name){
	bool file_exists;
	file_exists = check_if_file_exists(name);
	if(file_exists == false){
		send_uart("No existe registro de personas!!!\n\n");
		send_uart("Creando registro nuevo.\n\n");
		create_file(name);
	}
	return;
}

static bool check_if_user_exists_on_database(char *name, char *user_sequence, char *user_name){
	char *current_user_sequence;
	bool user_exists = false;
	open_file(name);
	get_line_from_file(user_name, 50);										/*Descarto el header del archivo*/
	while(get_line_from_file(user_name, 50) != 0){							//Se rompe el while al llegar al final del archivo
		user_name = strtok(user_name, " ");									//Usando este delimitador consigo primero el usuario de esa linea
		current_user_sequence = strtok(NULL, ",");						//Luego se consigue la secuencia asociada de esa linea
		if(current_user_sequence != NULL){
			if(strcmp(current_user_sequence, user_sequence) == 0){		//De hallarse una secuencia igual a la ingresada el usuario existe
				user_exists = true;
				break;
			}
		}
	}
	close_file(name);
	return user_exists;
}

static void save_current_recording(char *filename, uint16_t *buffer, uint32_t buffer_size){
	write_chunk(filename, buffer, sizeof(uint16_t), buffer_size);
	return;
}

static void delete_current_recording(char *filename){
	remove_file(filename);
	return;
}



static void process_recording_chunk(char *filename, float32_t *result, uint32_t result_size){
	static uint32_t current_pos = 0;
	uint16_t *ui_buffer = pvPortMalloc(CHUNK_SIZE * sizeof(uint16_t));
	float32_t *f_buffer = pvPortMalloc(CHUNK_SIZE * sizeof(float32_t));
	float32_t *aux_buffer;
	current_pos = read_chunk(filename, current_pos,ui_buffer, sizeof(uint16_t), CHUNK_SIZE);
	/*Convierto a float el bloque leido*/
	for(uint32_t i = 0; i < CHUNK_SIZE; i++){
		f_buffer[i] = ((float32_t) ui_buffer[i]) * 3.3 / ADC_RESOLUTION;
	}
	/*Libero la memoria utilizada por el bloque uint16_t*/
	vPortFree(ui_buffer);
	/*Asigno memoria para buffer auxiliar*/
	aux_buffer = pvPortMalloc(CHUNK_SIZE * sizeof(float32_t));




}

static void receive_sequence(char *sequence_buffer){
	for(uint8_t i = 0; i < SEQUENCE_LENGTH; i++){
		xQueueReceive(sequence_queue, &sequence_buffer[i], portMAX_DELAY);
	}
}

/*================[Public task]==========================*/
void user_check_task(void *pvParameters){
	char access_sequence[SEQUENCE_LENGTH + 1];
	char *user_name;
	bool user_exists = false;
	uint16_t test_buffer[4];
	uint32_t current_pos = 0;
	/*Realizo chequeo de archivos necesarios en la SD*/
	mount_sd("/");
	check_if_database_exists("database_personas.txt");
	check_if_register_exists("registro_accesos.txt");
	unmount_sd("/");
	while(1){
		receive_sequence(access_sequence);
		mount_sd("/");
		user_name = pvPortMalloc(50 * sizeof(char));
		user_exists = check_if_user_exists_on_database("database_personas.txt", access_sequence, user_name);
		if(user_exists){
			current_pos = read_chunk("current_voice.txt", current_pos, test_buffer, sizeof(uint16_t), 4);
			current_screen = PANTALLA_ACCESO_CONCEDIDO;
			send_screen_to_display(current_screen);

		}
		else{
			current_screen = PANTALLA_USUARIO_NO_EXISTE;
			send_screen_to_display(current_screen);
		}
		unmount_sd("/");
		vPortFree(user_name);
	}

}

void test_task(void *pvParameters){
	uint32_t current_pos = 0;
	uint16_t hardcode_buffer[] = {5, 4, 3 ,5, 8, 9, 10, 11};
	uint16_t test_buffer[4];
	mount_sd("/");
	while(1){
		for(uint8_t i = 0; i < 2; i++){
			write_chunk("hola.txt", hardcode_buffer + (i * 4), sizeof(uint16_t), 4);
		}

		current_pos = read_chunk("hola.txt", current_pos, test_buffer, sizeof(uint16_t), 4);
		current_pos = read_chunk("hola.txt", current_pos, test_buffer, sizeof(uint16_t), 4);
		remove_file("hola.txt");
		if(current_pos == 0){
			current_pos = 1;
		}
	}
}



