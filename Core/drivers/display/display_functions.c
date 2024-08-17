#include "main.h"
#include "cmsis_os.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "display_functions.h"
#include <string.h>
#include <stdio.h>
/*================[Extern variables]======================*/
extern QueueHandle_t display_queue;
/*================[Public functions]=====================*/
void send_screen_to_display(display_state_t current_screen){
	xQueueSend(display_queue, (void * ) &current_screen, portMAX_DELAY);
	return;
}




