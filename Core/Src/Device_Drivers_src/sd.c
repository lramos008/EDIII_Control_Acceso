/*Agrego includes necesarios*/
#include "File_Handling_RTOS.h"
#include "stm32f4xx_hal.h"
#include <string.h>
/*Traigo variables importantes*/
extern FATFS fs;  // file system
extern FIL fil; // File
extern FILINFO fno;
extern FRESULT fresult;  // result
extern UINT br, bw;  // File read/write count
extern RTC_HandleTypeDef hrtc;			//Handler del RTC
extern UART_HandleTypeDef huart2;

fileState verifyAccessRegister(char *name){
	/*Esta funcion es un wrapper nada mas*/
	/**** chequea si existe o no el archivo ****/
	fileState State;
	char *buf = pvPortMalloc(100*sizeof(char));
	Mount_SD("/");
	fresult = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fresult){
	case FR_OK:
		State = FILE_EXISTS;
		sprintf(buf, "*%s* existe en SD. OK.\n", name);
		Send_Uart(buf);
		break;
	case FR_NO_FILE:
	case FR_NO_PATH:
		State = FILE_DOESNT_EXIST;
		sprintf(buf, "*%s* no existe en SD!!!\nCreating new access register\n", name);
		Send_Uart(buf);
		Create_File(name);
		break;
	default:
		sprintf(buf, "Ocurrio un error desconocido!!!");
		Send_Uart(buf);
	}
	Unmount_SD("/");
	vPortFree(buf);
	return State;
}

fileState verifyDatabase(char *name){
	fileState State;
	char *buf = pvPortMalloc(100*sizeof(char));
	Mount_SD("/");
	fresult = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fresult){
	case FR_OK:
		State = FILE_EXISTS;
		sprintf(buf, "*%s* existe en SD. OK.\n", name);
		Send_Uart(buf);
		break;
	case FR_NO_FILE:
	case FR_NO_PATH:
		State = FILE_DOESNT_EXIST;
		sprintf(buf, "*%s* no existe en SD!!! Cargar base de datos y reiniciar\n", name);
		Send_Uart(buf);
		break;
	default:
		sprintf(buf, "Ocurrio un error desconocido!!!");
		Send_Uart(buf);
	}
	Unmount_SD("/");
	vPortFree(buf);
	if(State == FILE_DOESNT_EXIST){
		while(1);
	}
	return State;
}

char *searchUserOnDatabase(char *userSequence, char *databaseName){
	/***Esta funcion busca en una base de datos .txt al usuario asociado a la secuencia ingresada.***/
	/***Si lo encuentra, devuelve puntero al usuario (str). Si no, devuelve USER_ERROR.***/
	/***Se devuelve FILE_ERROR si no se puede abrir el archivo correctamente.***/
	/*Reservo espacio en memoria*/
	char *buf = pvPortMalloc(100*sizeof(char));
	char *userName;
	char *currentUserKey;
	Mount_SD("/");
	fresult = f_open(&fil, databaseName, FA_READ);
	if (fresult != FR_OK){
		/*Esto se implementa con fines de debugging*/
		sprintf (buf, "Error al abrir archivo *%s*\n\n", databaseName);
		Send_Uart(buf);
		vPortFree(buf);
		return FILE_ERROR;
	}
	/*Comienza la busqueda del usuario*/
	f_gets(buf, 100, &fil);												//Descarto header del archivo
	while(f_gets(buf, 100, &fil)){										//Avanza linea a linea del archivo hasta el final
		userName = strtok(buf, " ");									//Usando este delimitador consigo primero el usuario
		currentUserKey = strtok(NULL, ",");								//Luego consigo la clave, que viene despues del espacio
		if(currentUserKey != NULL && strcmp(currentUserKey, userSequence) == 0){
			Unmount_SD("/");
			return userName;											/*Recordar liberar memoria de userName en la tarea*/
		}
	}
	/*Libero memoria y desmonto tarjeta SD*/
	vPortFree(buf);
	Unmount_SD("/");
	return USER_ERROR;	/*No existe el usuario*/
}

char *get_time_from_rtc(char *rtc_lecture){
	/***Esta funcion devuelve en formato string la fecha y hora actual usando el RTC***/
	RTC_TimeTypeDef currentTime;
	RTC_DateTypeDef currentDate;
	char *time = pvPortMalloc(15 * sizeof(char));
	char *date = pvPortMalloc(30 * sizeof(char));
	/*Obtengo el tiempo actual*/
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	/*Obtengo la fecha actual*/
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	/*Guardo fecha y hora en los buffers correspondientes*/
	snprintf(time, 15, "%02d:%02d:%02d", currentTime.Hours, currentTime.Minutes, currentTime.Seconds);
	snprintf(date, 15, "%02d-%02d-%2d ", currentDate.Date, currentDate.Month, 2000 + currentDate.Year);
	strcat(date, time);
	vPortFree(time);
	return date;							/*Liberar memoria luego de usar la fecha y hora*/
}

void write_on_file(char *filename, char *content){
	Mount_SD("/");
	Update_File(filename, content);
	Unmount_SD("/");
	return;
}

void recordOnRegister(char *regName, char *userName, char *accessSequence){
	char regEntry[100];
	char *dateAndTime;
	/*Obtengo fecha y hora*/
	dateAndTime = getTimeFromRTC();
	/*Armo la entrada del registro*/
	snprintf(regEntry, 100, "%s %s %s\n", dateAndTime, userName, accessSequence);
	/*Escribo en la tarjeta SD*/
	write_on_file(regName, regEntry);
	/*Libero memoria utilizada*/
	vPortFree(dateAndTime);
	vPortFree(userName);								//Libero memoria del puntero buf creado
														//en la funcion que busca usuario
	return;
}







