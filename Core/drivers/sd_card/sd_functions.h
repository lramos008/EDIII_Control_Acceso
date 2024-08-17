#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H
#include "ff.h"
#include <stdbool.h>
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
bool check_if_file_exists(char *name);
char *get_line_from_file(char *buffer, int buffer_length);
void get_time_from_rtc(char *rtc_lecture);

#endif // SD_FUNCTIONS_H
