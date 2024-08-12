#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

/*===============[Public functions]=====================*/
void mount_sd(const char* path);
void unmount_sd(const char* path);
FRESULT open_file(char *name);
FRESULT close_file(char *name);
FRESULT write_file(char *name, char *data);
FRESULT read_file(char *name, char *buffer);
FRESULT append_to_file(char *name, char *data);
void get_line_from_file(char *buffer, int buffer_length);

#endif // SD_FUNCTIONS_H
