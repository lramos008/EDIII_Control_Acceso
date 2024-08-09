#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "tasks.h"

FSM StateMachine;
extern QueueHandle_t uiQueue;
extern QueueHandle_t sequenceQueue;
extern QueueHandle_t lockQueue;

void VoiceProcessing(void *pvParameters){

}

void KeypadScanning (void *pvParameters){
	eventoDisplay eventoEnviado;
	uint8_t keyPressed;
	FSM *FSM1 = &StateMachine;
	FSM1->CurrentState = ESPERA_DIGITO_1;
	/*Inicio el funcionamiento con la pantalla inicial*/
	eventoEnviado = PANTALLA_INGRESE_CLAVE;
	xQueueSend(uiQueue, (void * ) &eventoEnviado, portMAX_DELAY);
	while(1){
		switch(FSM1->CurrentState){
		case ESPERA_DIGITO_1:
			/*Parseo la primer tecla*/
			keyPressed = GetKey();
			if(keyPressed != 0){
            	if((keyPressed != BORRAR_DIGITO) && (keyPressed != ENTER)){
					FSM1->CurrentIndex = 0;												//Reinicializo indice del buffer
					FSM1->Buffer[FSM1->CurrentIndex] = keyPressed;						//Guardo el primer digito
					FSM1->CurrentIndex++;
					FSM1->StartTime = xTaskGetTickCount();								//Guardo tiempo de inicio para el timeout
					FSM1->CurrentState = ESPERA_DIGITO_2;								//Config estado para parsear la sig tecla
					eventoEnviado = PANTALLA_AÑADIR_ENTRADA;							//Defino indicaciones para el display
            	}
			}
			else{
				eventoEnviado = PANTALLA_INGRESE_CLAVE;
			}
			break;
		case ESPERA_DIGITO_2:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_3:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_4:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_5:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_6:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_ENTER:
			if((xTaskGetTickCount() - FSM1->StartTime) < pdMS_TO_TICKS(KEYPAD_TIMEOUT)){
				keyPressed = GetKey();
				if(keyPressed != 0){
					if(keyPressed == BORRAR_DIGITO){
						FSM1->CurrentIndex--;
						FSM1->Buffer[FSM1->CurrentIndex] = 0;					//El digito obtenido en el estado anterior lo borro
						FSM1->StartTime = xTaskGetTickCount();					//Tomo nueva referencia para calcular timeout
						FSM1->CurrentState--;									//Vuelvo al estado anterior
						eventoEnviado = PANTALLA_BORRAR_ENTRADA;
					}
					else{
						if(keyPressed == ENTER){
							for(uint8_t i = 0; i < SEQUENCE_LEN; i++){
								xQueueSend(sequenceQueue, &(FSM1->Buffer[i]), portMAX_DELAY);
							}
							ClearBuffer(FSM1->Buffer, 6);
							FSM1->CurrentState = ESPERA_DIGITO_1;
						}
					}
				}
				else{
					eventoEnviado = PANTALLA_IDLE;								//No actualizar la pantalla
				}
			}
			else{
				FSM1->CurrentState = TIMEOUT;
				eventoEnviado = PANTALLA_TIMEOUT;

			}
			break;
		case TIMEOUT:
			ClearBuffer(FSM1->Buffer, 6);								//Limpio buffer y vuelvo a estado inicial
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		case INCOMPLETO:
			ClearBuffer(FSM1->Buffer, 6);
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		default:
			ClearBuffer(FSM1->Buffer, 6);
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		}
		/*Si el evento es PANTALLA_IDLE no se envian indicaciones al display*/
		/*En ese caso se deja la pantalla previa*/
		if(eventoEnviado != PANTALLA_IDLE) xQueueSend(uiQueue, (void * ) &eventoEnviado, portMAX_DELAY);
	}
}

void ScreenManager  (void *pvParameters){
	lockState magneticLockState;
	eventoDisplay eventoRecibido = PANTALLA_INGRESE_CLAVE;
	uint8_t x = 10;														//x e y definen coordenadas en pantalla
	uint8_t y = 26;
	displayInit();
	while(1){
		xQueueReceive(uiQueue, &eventoRecibido, portMAX_DELAY);			//La tarea espera indicaciones para cambiar la pantalla
		switch(eventoRecibido){
		case PANTALLA_INGRESE_CLAVE:
			x = 10;
			displayInsertKeyMsg();
			break;
		case PANTALLA_AÑADIR_ENTRADA:
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString("*", Font_16x26, White);				//Añade *'s para representar ingreso de digitos
			ssd1306_UpdateScreen();
			x += 16;													//Avanzo una posición en pantalla acorde al font seleccionado
			break;
		case PANTALLA_BORRAR_ENTRADA:
			x -= 16;													//Retrocedo una posicion en pantalla acorde al font seleccionado
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString(" ", Font_16x26, White);				//En caso de borrar relleno con espacio en blanco
			ssd1306_UpdateScreen();
			break;
		case PANTALLA_TIMEOUT:
			displayTimeoutMsg();
			HAL_Delay(3000);											//Timer bloqueante para evitar el ingreso de otra secuencia
			break;														//mientras se muestra el mensaje.
		case PANTALLA_SECUENCIA_INCOMPLETA:
			displayIncompleteMsg();
			HAL_Delay(3000);
			break;
		case PANTALLA_ACCESO_CONCEDIDO:
			displayAccessState();
			magneticLockState = LOCK_ON;
			HAL_Delay(3000);
			xQueueSend(lockQueue, &magneticLockState, portMAX_DELAY);
			break;
		case PANTALLA_USUARIO_INEXISTENTE:
			displayNonExistentUserMsg();
			HAL_Delay(3000);
			break;
		case PANTALLA_IDLE:
			/*No hacer nada*/
			break;
		default:
			break;
		}
	}
}

void sdHandler(void *pvParameters){
	/***Esta tarea se encarga del manejo de la memoria SD***/
	eventoDisplay eventoEnviado;
	lockState magneticLockState= LOCK_ON;
	char accessSequence[SEQUENCE_LEN + 1];
	accessSequence[6] = 0;
	char *userName;
	/*Realizo verificacion de archivos inicial*/
	verifyDatabase("database_personas.txt");
	//verifyDatabaseFFT("database_fft.txt");
	verifyAccessRegister("registro_accesos.txt");
	while(1){
		/*Espero secuencia de 6 digitos*/
		for(uint8_t i = 0; i < SEQUENCE_LEN; i++){
			xQueueReceive(sequenceQueue, &accessSequence[i], portMAX_DELAY);
		}
		/*Verifico que la secuencia este en la base de datos de personas*/
		userName = searchUserOnDatabase(accessSequence, "database_personas.txt");
		if(userName == USER_ERROR){
			userName = pvPortMalloc(25 * sizeof(char));
			snprintf(userName, 25, "Usuario desconocido");
			recordOnRegister("registro_accesos.txt", userName, accessSequence);
			eventoEnviado = PANTALLA_USUARIO_INEXISTENTE;
			xQueueSend(uiQueue, &eventoEnviado, portMAX_DELAY);
		}
		else{
			recordOnRegister("registro_accesos.txt", userName, accessSequence);
			magneticLockState = LOCK_OFF;
			eventoEnviado = PANTALLA_ACCESO_CONCEDIDO;
			xQueueSend(lockQueue, &magneticLockState, portMAX_DELAY);				//Envio indicaciones a la cerradura
			xQueueSend(uiQueue, &eventoEnviado, portMAX_DELAY);						//Envio indicaciones al display
		}															//Libero memoria utilizada
	}
}

void LockControl(void *pvParameters){
	lockState magneticLockState = LOCK_ON;
	closeLock();
	while(1){
		xQueueReceive(lockQueue, &magneticLockState, portMAX_DELAY);
		if(magneticLockState == LOCK_OFF){
			openLock();
			xQueueReceive(lockQueue, &magneticLockState, portMAX_DELAY);			//Para sincronizar con la pantalla
			if(magneticLockState == LOCK_ON) closeLock();
		}
	}
}

void IdleTask(void *pvParameters){
	while(1){

	}
}
