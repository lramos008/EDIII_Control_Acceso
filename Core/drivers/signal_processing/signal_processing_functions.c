#define ARM_MATH_CM4
#include "arm_math.h"
#include <stdbool.h>

#define ADC_RESOLUTION 4096
#define NUM_TAPS 50

/*========================[FIR coefficients]==============================*/
float32_t fir_coeffs[NUM_TAPS] = {};

/*========================[Private variables]=============================*/
arm_fir_instance_f32 fir_processor;
arm_rfft_fast_instance_f32 fft_processor;
/*========================[Private functions]=============================*/
static void convert_to_signal(uint16_t *p_src, float32_t *p_dst, uint32_t block_size){
	for(uint32_t i = 0; i < block_size; i++){
		p_dst[i] = ((float32_t) p_src[i]) * 3.3 / ADC_RESOLUTION;
	}
	return;
}

static void filter_voice_signal(float32_t *p_src, float32_t *p_dst, uint32_t block_size){
	static bool is_filter_init = false;
	float32_t *state_array = pvPortMalloc((NUM_TAPS + block_size - 1) * sizeof(float32_t));
	/*Inicializo el filtro FIR si corresponde*/
	if(is_filter_init == false){
		arm_fir_init_f32(&fir_processor, NUM_TAPS, fir_coeffs, state_array, block_size);
		is_filter_init = true;
	}
	/*Aplico filtro FIR a los datos de entrada*/
	arm_fir_f32(&fir_processor, p_src, p_dst, block_size);
	vPortFree(state_array);
	return;
}

static void apply_hamming_window(float32_t *p_src, uint32_t block_size){
	// Constants for Hamming window
	const float32_t alpha = 0.54f;
	const float32_t beta = 0.46f;
	float32_t hamming_coeff;
	for (uint32_t i = 0; i < block_size; i++) {
		// Calculate the Hamming coefficient for the current sample
	    hamming_coeff = alpha - beta * arm_cos_f32((2.0f * PI * i) / (block_size - 1));
	    // Apply the Hamming coefficient to the signal sample
	    p_src[i] *= hamming_coeff;

	}
	return;
}

static void calculate_fft(float32_t *p_src, float32_t *p_dst, uint32_t block_size){
	static bool is_fft_init = false;
	/*Inicializo la instancia de fft si corresponde*/
	if(is_fft_init == false){
		arm_rfft_fast_init_f32(&fft_processor, block_size);
		is_fft_init = true;
	}
	/*Calculo la fft*/
	arm_rfft_fast_f32(&fft_processor, p_src, p_dst, 0);					//El cero indica que se hace transformada, no antitransformada
	return;
}

static void normalize_array(float32_t *p_src, uint32_t block_size){
	float32_t min_val, max_val;
	uint32_t min_index, max_index;
	float32_t range;
	/*Encuentro el maximo y el minimo del array*/
	arm_min_f32(p_src, block_size, &min_val, &min_index);
	arm_max_f32(p_src, block_size, &max_val, &max_index);
	/*Calculo el rango*/
	range = max_val - min_val;
	/*Normalizo el array*/
	for(uint32_t i = 0; i < block_size; i++){
		p_src[i] = (p_src[i] - min_val) / range;
	}
	return;
}

static void get_fft_norm_mag(float32_t *p_src, float32_t *p_dst, uint32_t block_size){
	/*Se tienen en cuenta la cantidad de numeros complejos, no de elementos en p_src*/
	/*Calculo la magnitud de la fft*/
	arm_cmplx_mag_f32(p_src, p_dst, block_size);
	/*Normalizo*/
	normalize_array(p_dst, block_size);
	return;
}

/*========================[Public functions]================================*/
void process_signal_frame(uint16_t *p_src, float32_t *p_dst, uint32_t block_size){
	float32_t *aux_buf_1 = pvPortMalloc(block_size * sizeof(float32_t));
	float32_t *aux_buf_2 = pvPortMalloc(block_size * sizeof(float32_t));
	/*Convierto valores del ADC en una señal*/
	convert_to_signal(p_src, aux_buf, block_size);
	/*Filtro la señal. Frecuencia de corte de 4 kHz*/
	filter_voice_signal(aux_buf, aux_buf_2, block_size);
	/*Aplico ventana de Hamming para reducir spectral leakage*/
	apply_hamming_window(aux_buf_2, block_size);
	/*Calculo la fft*/
	calculate_fft(aux_buf_2, aux_buf_1, block_size);
	/*Obtengo la magnitud de la fft normalizada*/
	get_fft_norm_mag(aux_buf_1, p_dst, block_size/2);
	/*Libero memoria utilizada*/
	vPortFree(aux_buf_1);
	vPortFree(aux_buf_2);
	return;
}

bool compare_features(float32_t *p_src, float32_t *p_dst, uint32_t block_size){
	bool voice_recognized = false;

}


