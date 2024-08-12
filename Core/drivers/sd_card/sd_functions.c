#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "fatfs_sd.h"

#define UART &huart2

FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;			//Handler del RTC


static void send_uart(char *string){
	HAL_UART_Transmit(UART, (uint8_t *)string, strlen (string), HAL_MAX_DELAY);
	return;
}

void mount_sd(const char* path){
	fresult = f_mount(&fs, path, 1);
	if (fresult != FR_OK){
		send_uart("Error al montar la tarjeta SD!!!\n\n");
		while(1);
	}
	return;
}

void unmount_sd(const char* path){
	fresult = f_mount(NULL, path, 1);
	if(fresult != FR_OK){
		send_uart("Error al desmontar la tarjeta SD!!!\n\n");
		while(1);
	}
}

FRESULT write_file(char *name, char *data){
	/*Chequeo existencia del archivo*/
	fresult = f_stat (name, &fno);
	if(fresult != FR_OK){
		send_uart("Error!!! El archivo no existe.\n\n");
		while(1);
	}
	/*Abro el archivo*/
	fresult = f_open(&fil, name, FA_OPEN_EXISTING | FA_WRITE);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo abrir el archivo.\n\n");
		while(1);
	}
	/*Escribo el archivo*/
	fresult = f_write(&fil, data, strlen(data), &bw);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo escribir el archivo.\n\n");
		while(1);
	}
	/*Cierro el archivo*/
	fresult = f_close(&fil);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo cerrar el archivo.\n\n");
		while(1);
	}
	return fresult;
}

FRESULT append_to_file(char *name, char *data){
	/*Chequeo existencia del archivo*/
	fresult = f_stat (name, &fno);
	if(fresult != FR_OK){
		send_uart("Error!!! El archivo no existe.\n\n");
		while(1);
	}
	/*Abro el archivo*/
	fresult = f_open(&fil, name, FA_OPEN_APPEND | FA_WRITE);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo abrir el archivo.\n\n");
		while(1);
	}
	/*Escribo el archivo*/
	fresult = f_write(&fil, data, strlen(data), &bw);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo escribir el archivo.\n\n");
		while(1);
	}
	/*Cierro el archivo*/
	fresult = f_close(&fil);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo cerrar el archivo.\n\n");
		while(1);
	}
	return fresult;
}



FRESULT read_file(char *name, char *buffer){
	/*Chequeo existencia del archivo*/
	fresult = f_stat (name, &fno);
	if(fresult != FR_OK){
		send_uart("Error!!! El archivo no existe.\n\n");
		while(1);
	}
	/*Abro el archivo*/
	fresult = f_open(&fil, name, FA_READ);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo abrir el archivo.\n\n");
		while(1);
	}
	fresult = f_read(&fil, buffer, f_size(&fil), &br);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo leer el archivo.\n\n");
		while(1);
	}
	/* Close file */
	fresult = f_close(&fil);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo leer el archivo.\n\n");
		while(1);
	}
	return fresult;
}

FRESULT open_file(char *name){
	/*Chequeo existencia del archivo*/
	fresult = f_stat (name, &fno);
	if(fresult != FR_OK){
		send_uart("Error!!! El archivo no existe.\n\n");
		while(1);
	}
	/*Abro el archivo*/
	fresult = f_open(&fil, name, FA_READ);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo abrir el archivo.\n\n");
		while(1);
	}
	return fresult;
}

FRESULT close_file(char *name){
	/* Close file */
	fresult = f_close(&fil);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo leer el archivo.\n\n");
		while(1);
	}
	return fresult;
}

void get_line_from_file(char *buffer, int buffer_length){
	f_gets(buffer, buffer_length, &fil);
	return;
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


