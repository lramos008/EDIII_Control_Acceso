#include "main.h"
#include "cmsis_os.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "display_functions.h"
#include <string.h>
#include <stdio.h>

/*================[Extern variables]======================*/
extern QueueHandle_t display_queue;

typedef enum{
	AGREGAR_DIGITO = 0,
	BORRAR_DIGITO,
	REINICIAR_ENTRADA
}entry_t;

/*================[Private functions]=====================*/
static void display_init(void){
	/*Wrapper de la función que inicializa el display*/
	ssd1306_Init();
	return;
}

static void display_sequence_entry(entry_t action){
	static uint8_t x = 10;														//x e y definen coordenadas en pantalla
	static uint8_t y = 26;
	if(action == AGREGAR_DIGITO){
		ssd1306_SetCursor(x, y);
		ssd1306_WriteString("*", Font_16x26, White);				//Añade *'s para representar ingreso de digitos
		ssd1306_UpdateScreen();
		x += 16;
	}
	else{
		if(action == BORRAR_DIGITO){
			x -= 16;													//Retrocedo una posicion en pantalla acorde al font seleccionado
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString(" ", Font_16x26, White);				//En caso de borrar relleno con espacio en blanco
			ssd1306_UpdateScreen();
		}
		else{
			/*Reiniciar entrada*/
			x = 10;
			ssd1306_Fill(Black);
			ssd1306_SetCursor(x, y);
			ssd1306_UpdateScreen();
		}
	}
}

static void display_insert_key_msg(void){
	uint8_t x = 5;
	uint8_t y = 0;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Ingrese", Font_16x26, White);
	x += 15;
	y += 26;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("clave", Font_16x26, White);
	ssd1306_UpdateScreen();
	ssd1306_Fill(Black);								//Para preparar la sig secuencia
}

static void display_timeout_msg(void){
	uint8_t x = 5;
	uint8_t y = 18;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Timeout!!!", Font_11x18, White);
	ssd1306_UpdateScreen();
	vTaskDelay(3000 / portTICK_RATE_MS);
	display_sequence_entry(REINICIAR_ENTRADA);
}

static void display_incomplete_msg(void){
	uint8_t x = 20;
	uint8_t y = 0;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Ingrese", Font_11x18, White);
	x = 10;
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("secuencia", Font_11x18, White);
	x = 20;
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("valida", Font_11x18, White);
	ssd1306_UpdateScreen();
	vTaskDelay(3000 / portTICK_RATE_MS);
	display_sequence_entry(REINICIAR_ENTRADA);

}

static void display_access_granted(void){
	uint8_t x = 5;
	uint8_t y = 0;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Acceso", Font_11x18, White);
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("concedido", Font_11x18, White);
	ssd1306_UpdateScreen();
	vTaskDelay(3000/portTICK_RATE_MS);
}

static void display_no_user_found(void){
	uint8_t x = 20;
	uint8_t y = 0;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Usuario", Font_11x18, White);
	x = 20;
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("no", Font_11x18, White);
	x = 20;
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("existente", Font_11x18, White);
	ssd1306_UpdateScreen();
	vTaskDelay(3000/portTICK_RATE_MS);
	display_sequence_entry(REINICIAR_ENTRADA);
}

static display_state_t receive_screen(void){
	display_state_t received_screen;
	xQueueReceive(display_queue, &received_screen, portMAX_DELAY);
	return received_screen;
}

/*================[Public tasks]=========================*/
void display_manager(void *pvParameters){
	display_state_t current_screen;
	display_init();
	while(1){
		current_screen = receive_screen();
		switch(current_screen){
		case PANTALLA_DE_INICIO:
			display_insert_key_msg();
			break;
		case PANTALLA_AGREGAR_ENTRADA:
			display_sequence_entry(AGREGAR_DIGITO);
			break;
		case PANTALLA_BORRAR_ENTRADA:
			display_sequence_entry(BORRAR_DIGITO);
			break;
		case PANTALLA_BUSQUEDA_DE_USUARIO:
			display_sequence_entry(REINICIAR_ENTRADA);
			break;
		case PANTALLA_ACCESO_CONCEDIDO:
			display_access_granted();
			break;
		case PANTALLA_USUARIO_NO_EXISTE:
			display_no_user_found();
			break;
		case PANTALLA_TIMEOUT:
			display_timeout_msg();
			break;
		case PANTALLA_SECUENCIA_INCOMPLETA:
			display_incomplete_msg();
			break;
		case PANTALLA_IDLE:
		default:
			break;
		}
	}
}
