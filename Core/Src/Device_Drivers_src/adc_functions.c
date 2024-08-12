#include "Device_Drivers_inc/adc_functions.h"


extern ADC_HandleTypeDef hadc1;
/*Defino coeficientes del filtro fir*/
float32_t filter_coefficients[NUM_TAPS] = {
  0.0016534408745193917, 0.00971344278000464, 0.0010795824650909253, -0.006120130830812831, 0.00062623050176212,
  0.007756836409234425, -0.0029135926296114337, -0.009308404897096138, 0.006067440669258805, 0.010474706625638526,
  -0.010372352319983355, -0.010901822761108843, 0.016010043696733908, 0.010089526168680654, -0.023247547984607944,
  -0.007368996888129218, 0.03261559131101637, 0.0015911965903157629, -0.0453886385479251, 0.009759331132837116,
  0.065467263723442, -0.034603239932210525, -0.10996167118462251, 0.12279136336080168, 0.47410057840416264,
  0.47410057840416264, 0.12279136336080168, -0.10996167118462251, -0.034603239932210525, 0.065467263723442,
  0.009759331132837116, -0.0453886385479251, 0.0015911965903157629, 0.03261559131101637, -0.007368996888129218,
  -0.023247547984607944, 0.010089526168680654, 0.016010043696733908, -0.010901822761108843, -0.010372352319983355,
  0.010474706625638526, 0.006067440669258805, -0.009308404897096138, -0.0029135926296114337, 0.007756836409234425,
  0.00062623050176212, -0.006120130830812831, 0.0010795824650909253, 0.00971344278000464, 0.0016534408745193917
};


arm_fir_instance_f32 fir_processor;
arm_rfft_fast_instance_f32 fft_processor;
arm_status status;

/**
 * @brief Aplica un filtro FIR a los datos de entrada.
 *
 * Esta función filtra los datos de entrada utilizando un conjunto de coeficientes FIR
 * predefinidos y almacena el resultado en el búfer de salida.
 *
 * @param psrc	 Puntero a los datos de entrada.
 * @param pdst	 Puntero al buffer donde se almacenarán los datos filtrados.
 *
 */

void captureVoice(uint16_t *buffer, uint32_t bufferSize){
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*) buffer, bufferSize);
	vTaskDelay(2000/portTICK_RATE_MS);
	return;
}

void save_voice_on_sd(uint16_t *buffer, uint32_t buffer_size){

}
static void apply_fir_filter(float32_t *psrc, float32_t *pdst){
	static uint8_t is_filter_initialized = 0;
	float32_t state_array[STATE_SIZE];
	/*Verifico si el filtro ya esta inicializado*/
	if(!is_filter_initialized){
		arm_fir_init_f32(&fir_processor, NUM_TAPS, filter_coefficients, state_array, BLOCK_SIZE);
		is_filter_initialized = 1;
	}
	/*Aplico filtro fir a los datos de entrada psrc*/
	arm_fir_f32(&fir_processor, psrc, pdst, BLOCK_SIZE);
	return;
}

static void apply_hamming_window(float32_t *psrc){
	for(uint32_t i = 0; i < BLOCK_SIZE; i++){
		psrc[i] *= (0.54 - 0.46 * arm_cos_f32(2 * PI * i / (BLOCK_SIZE - 1)));
	}
	return;
}

void normalize_array(float32_t *psrc, uint32_t size){
	float32_t min_val, max_val;
	uint32_t min_index, max_index;
	float32_t range;
	/*Encuentro el maximo y el minimo del array*/
	arm_min_f32(psrc, size, &min_val, &min_index);
	arm_max_f32(psrc, size, &max_val, &max_index);
	/*Calculo el rango*/
	range = max_val - min_val;
	/*Normalizo el array*/
	for(uint32_t i = 0; i < size; i++){
		psrc[i] = (psrc[i] - min_val) / range;
	}
	return;
}

static void get_fft_mag(float32_t *psrc, float32_t *pdst){
	float32_t fft_buffer[BLOCK_SIZE];
	static uint8_t is_fft_initialized = 0;
	if(!is_fft_initialized){
		arm_rfft_fast_init_f32(&fft_processor, BLOCK_SIZE);
		is_fft_initialized = 1;
	}
	/*Realizo transformada de fourier de este bloque*/
	arm_rfft_fast_f32(&fft_processor, psrc, fft_buffer, 0);		//Cada elemento son pares parte real y parte imaginaria
	/*Calculo la magnitud de la transformada de fourier.*/
	arm_cmplx_mag_f32(fft_buffer, pdst, BLOCK_SIZE/2);			//Al calcular magnitud el array se divide a la mitad (real + cmplx ->> magnitud)
	/*Normalizo*/
	normalize_array(pdst, BLOCK_SIZE/2);
	return;
}

void process_voice(float32_t *psrc, float32_t *pdst){
	float32_t aux_buffer[BLOCK_SIZE];
	/*Aplico filtro FIR al bloque*/
	apply_fir_filter(psrc, aux_buffer);
	/*Aplico ventana de hamming al bloque*/
	apply_hamming_window(aux_buffer);
	/*Obtengo las magnitudes normalizadas de la fft*/
	get_fft_mag(aux_buffer, pdst);
	return;
}





