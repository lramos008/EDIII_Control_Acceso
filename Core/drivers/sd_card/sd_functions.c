#include "main.h"
#include "fatfs.h"
#include "ff.h"
#include "string.h"
#include "stdio.h"
#include "fatfs_sd.h"
#include <stdbool.h>

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


void send_uart(char *string){
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

bool check_if_file_exists(char *name){
	bool file_exist = false;
	fresult = f_stat(name, &fno);
	if(fresult == FR_OK){
		file_exist = true;
	}
	return file_exist;
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
		send_uart("Error!!! No se pudo cerrar el archivo.\n\n");
		while(1);
	}
	return fresult;
}

FRESULT create_file(char *name){
	/*Chequeo existencia del archivo*/
	fresult = f_stat (name, &fno);
	if(fresult != FR_OK){
		send_uart("Error!!! El archivo no existe.\n\n");
		while(1);
	}
	/*Abro el archivo y lo creo*/
	fresult = f_open(&fil, name, FA_CREATE_ALWAYS|FA_READ|FA_WRITE);
	if(fresult != FR_OK){
		send_uart("Error!!! No se pudo crear el archivo.\n\n");
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

char *get_line_from_file(char *buffer, int buffer_length){
	return f_gets(buffer, buffer_length, &fil);
}

void get_time_from_rtc(char *rtc_lecture){
	/***Esta funcion devuelve en formato string la fecha y hora actual usando el RTC***/
	RTC_TimeTypeDef currentTime;
	RTC_DateTypeDef currentDate;
	char *time = pvPortMalloc(15 * sizeof(char));
	/*Obtengo el tiempo actual*/
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	/*Obtengo la fecha actual*/
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	/*Guardo fecha y hora en los buffers correspondientes*/
	snprintf(time, 15, "%02d:%02d:%02d", currentTime.Hours, currentTime.Minutes, currentTime.Seconds);
	snprintf(rtc_lecture, 15, "%02d-%02d-%2d ", currentDate.Date, currentDate.Month, 2000 + currentDate.Year);
	strcat(rtc_lecture, time);
	vPortFree(time);
	return;
}



