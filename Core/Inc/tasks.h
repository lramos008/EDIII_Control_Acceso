#ifndef TASKS_H
#define TASKS_H

#include "keypad.h"
#include "screen.h"
#include "sd.h"
#include "lock.h"
/*Defino prototipos de las tareas*/
void VoiceProcessing(void *pvParameters);
void KeypadScanning (void *pvParameters);
void ScreenManager  (void *pvParameters);
void LockControl    (void *pvParameters);
void sdHandler(void *pvParameters);
void IdleTask       (void *pvParameters);
#endif /* TASKS_H */
