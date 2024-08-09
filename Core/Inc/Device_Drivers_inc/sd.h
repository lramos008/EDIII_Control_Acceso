#ifndef SD_DRIVER_H
#define SD_DRIVER_H
/*Agrego includes necesarios*/
#include "File_Handling_RTOS.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define UART &huart2
#define verifyDatabaseFFT(name) verifyDatabase(name)
#define SEQUENCE_LEN 6
#define USER_ERROR 0
#define FILE_ERROR 0
typedef enum{
	FILE_EXISTS = 0,
	FILE_DOESNT_EXIST,
}fileState;



fileState fileIsOnSD(char *name);
fileState verifyDatabase(char *name);
char *searchUserOnDatabase(char *userSequence, char *databaseName);
fileState verifyAccessRegister(char *name);
void recordOnRegister(char *regName, char *userName, char *accessSequence);
char *getTimeFromRTC(void);


#endif /* SD_DRIVER_H */
