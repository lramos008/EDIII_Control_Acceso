#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H
/*================[Public data types]=============================*/
typedef enum{
	PANTALLA_IDLE = 0,
	PANTALLA_DE_INICIO,
	PANTALLA_AGREGAR_ENTRADA,
	PANTALLA_BORRAR_ENTRADA,
	PANTALLA_BUSQUEDA_DE_USUARIO,
	PANTALLA_ACCESO_CONCEDIDO,
	PANTALLA_USUARIO_NO_EXISTE,
	PANTALLA_TIMEOUT,
	PANTALLA_SECUENCIA_INCOMPLETA
}display_state_t;
/*================[Public function prototypes]=====================*/
void send_screen_to_display(display_state_t current_screen);
#endif /* DISPLAY_FUNCTIONS_H */
