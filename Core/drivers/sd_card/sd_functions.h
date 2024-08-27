#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H
#include "ff.h"
#include <stdbool.h>

/*===============[Public data types]====================*/

/*===============[Public functions]=====================*/
void send_uart(char *string);
void mount_sd(const char* path);
void unmount_sd(const char* path);
FRESULT open_file(char *name);
FRESULT close_file(char *name);
FRESULT write_file(char *name, char *data);
FRESULT read_file(char *name, char *buffer);
FRESULT append_to_file(char *name, char *data);
FRESULT create_file(char *name);
bool write_buffer_on_sd(char *name, void *data, size_t element_size, size_t length);
bool check_if_file_exists(char *name);
char *get_line_from_file(char *buffer, int buffer_length);
void get_time_from_rtc(char *rtc_lecture);
uint32_t read_chunk(char *filename, uint32_t current_pos, void *buffer, size_t element_size, size_t length);
void write_chunk(char *filename, void *buffer, size_t data_type_size, size_t chunk_size);
FRESULT remove_file(char *filename);
#endif // SD_FUNCTIONS_H
