#include "main.h"
#include "cmsis_os.h"
#include "sd_functions.h"
#include "display_functions.h"
#include <string.h>
#include <stdbool.h>

/*================[Private defines]========================*/
#define SEQUENCE_LENGTH 6
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
	return user_exists;
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
			current_screen = PANTALLA_ACCESO_CONCEDIDO;
			send_screen_to_display(current_screen);

		}
		else{
			current_screen = PANTALLA_USUARIO_NO_EXISTE;
			send_screen_to_display(current_screen);
		}
		mount_sd("/");
		vPortFree(user_name);
	}

}



