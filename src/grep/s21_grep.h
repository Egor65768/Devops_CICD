#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags_grep {
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_o;
  int flag_f;
} FLAGS;

void search(char **argv, int pattern[], int n_pattern, int n, int fileCount,
           FLAGS flags, int *flag_error);
void parsing(int pattern[], int *n_pattern, FLAGS *flags, int argc, char **argv,
             int *flag_error, int *filename, int *fileCount, int *flag_have);
void checking_end_string(char *line);
void print_string_much_file(int flag_num, char *filename, char *line,
                           int num_str);
void print_string_one_file(int flag_num, char *line, int num_str);
void search_help(int n_pattern, char **argv, int pattern[], int options,
                char *line, int fileCount, FLAGS flags, int num_str,
                char *filename, int *kol_sovpad);
void checking_for_two_pattern(int n_pattern, int *i);
int checking_flag_e(int argc, char **argv);
void help_parsing_par_flag(char fl, FLAGS *flags, int *flag_flag,
                           int *flag_have, int argc, int pattern[],
                           int *n_pattern, int *i, int *flag_error);

void search_help_help(int ret, int fileCount, int n_pattern, FLAGS flags, int *i,
                     char *line, int num_str, char *filename, int flag_inv,
                     int *kol_sovpad);

#endif  // SRC_GREP_S21_GREP_H_