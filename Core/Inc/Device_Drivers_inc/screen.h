#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H
/*Agrego defines necesarios para manejar el display*/
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>

/*Agrego prototipos de funciones*/
void displayInit(void);
void displayInsertKeyMsg(void);
void displayTimeoutMsg(void);
void displayIncompleteMsg(void);
void displayAccessState(void);
void displayNonExistentUserMsg(void);

/*Agrego eventos de la pantalla al ingresar secuencia*/
typedef enum{
	PANTALLA_IDLE = 0,
	PANTALLA_INGRESE_CLAVE ,
	PANTALLA_AÑADIR_ENTRADA,
	PANTALLA_BORRAR_ENTRADA,
	PANTALLA_ACCESO_CONCEDIDO,
	PANTALLA_USUARIO_INEXISTENTE,
	PANTALLA_TIMEOUT,
	PANTALLA_SECUENCIA_INCOMPLETA
}eventoDisplay;

#endif /* SCREEN_DRIVER_H */
