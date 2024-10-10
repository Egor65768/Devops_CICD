#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printf_file(char *name, char *flags, int *index);
void parsing_flag(char *flags, int argc, char **argv, int *start_file_name,
                  int *flag_error);
void append_flag(char *flags, char flag);
void printf_sumbol(int c, char *flags, int *pologenie, int *index,
                   int *elem_in_str, int last_c, int *null_str, int *flag_skip);
void add_flag_in_array(char *flags, char **argv, int *flag_error, int i);
void processing_flag_v(char *flags, int *c);
void processing_flag_s(char *flags, int c, int *null_str, int *flag_skip,
                       int last_c);

#endif  // SRC_CAT_S21_CAT_H_