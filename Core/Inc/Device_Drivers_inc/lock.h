#ifndef LOCK_DRIVER_H
#define LOCK_DRIVER_H
/*Agrego includes necesarios*/
#include "main.h"
/*Agrego enums*/

typedef enum{
	LOCK_OFF = 0,
	LOCK_ON
}lockState;

/*Prototipos de funciones*/
void openLock(void);
void closeLock(void);
#endif /* LOCK_DRIVER_H */
