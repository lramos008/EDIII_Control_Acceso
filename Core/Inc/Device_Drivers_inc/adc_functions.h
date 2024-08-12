#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "main.h"
#define ARM_MATH_CM4
#include "arm_math.h"
#include "main.h"
#include "cmsis_os.h"

#define AUDIO_SAMPLES_LENGTH 16384						//La cantidad total de muestras de voz que deben tomarse
#define BUFFER_SIZE 4096								//Tamaño del buffer que recibe las muestras
#define BLOCK_SIZE	BUFFER_SIZE/2						//Tamaño de bloque que analizan las funciones de cmsis dsp
#define NUM_BLOCKS AUDIO_SAMPLES_LENGTH/BLOCK_SIZE		//Numero de bloques en total a analizar

#define NUM_TAPS 50
#define STATE_SIZE NUM_TAPS + BLOCK_SIZE - 1

void captureVoice(uint16_t *buffer, uint32_t bufferSize);

#endif //ADC_FUNCTIONS_H
