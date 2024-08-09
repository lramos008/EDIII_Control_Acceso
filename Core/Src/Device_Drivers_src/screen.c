#include "screen.h"
extern QueueHandle_t uiQueue;

void displayInit(void){
	/*Wrapper de la función que inicializa el display*/
	ssd1306_Init();
	return;
}

void displayInsertKeyMsg(void){
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

void displayTimeoutMsg(void){
	uint8_t x = 5;
	uint8_t y = 18;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Timeout!!!", Font_11x18, White);
	ssd1306_UpdateScreen();
}

void displayIncompleteMsg(void){
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

}

void displayAccessState(void){
	uint8_t x = 5;
	uint8_t y = 0;
	ssd1306_Fill(Black);
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("Acceso", Font_11x18, White);
	y += 18;
	ssd1306_SetCursor(x, y);
	ssd1306_WriteString("concedido", Font_11x18, White);
	ssd1306_UpdateScreen();
}

void displayNonExistentUserMsg(void){
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
}




