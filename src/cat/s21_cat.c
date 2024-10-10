#include "s21_cat.h"

int main(int argc, char **argv) {
  int flag_error = 0;
  char flag[7] = "\0";
  int start_file_name = 1;
  parsing_flag(flag, argc, argv, &start_file_name, &flag_error);
  if (flag_error == 0) {
    int index = 1;
    for (int i = start_file_name; i < argc; i++) {
      printf_file(argv[i], flag, &index);
    }
  } else {
    printf("flag_error\n");
  }
}

void printf_file(char *name, char *flags, int *index) {
  int position = 0;
  int elem_in_str = 0;
  FILE *f;
  f = fopen(name, "rt");
  if (f != NULL) {
    int last_c = '\n';
    int c = fgetc(f);
    int null_str = 0;
    int flag_skip = 0;
    while (c != EOF) {
      printf_sumbol(c, flags, &position, index, &elem_in_str, last_c, &null_str,
                    &flag_skip);
      last_c = c;
      c = fgetc(f);
    }
    if (last_c != '\n') {
      *index = *index + 1;
    }
    fclose(f);
  } else {
    printf("cat: %s: No such file or directory\n", name);
  }
}

void printf_sumbol(int c, char *flags, int *position, int *index,
                   int *elem_in_str, int last_c, int *null_str,
                   int *flag_skip) {
  processing_flag_s(flags, c, null_str, flag_skip, last_c);
  if (strchr(flags, 'n') != NULL && strchr(flags, 'b') == NULL &&
      *flag_skip != 1) {
    if (*position == 0) {
      printf("%6d\t", *index);
      *position = 1;
    }
    if (c == 10) {
      *index = *index + 1;
    }
  }
  if (strchr(flags, 'b') != NULL) {
    if (*position == 0 && !(*elem_in_str == 0 && c == 10)) {
      printf("%6d\t", *index);
      *position = 1;
    }
    if (c == 10 && *position != 0) {
      *index = *index + 1;
    }
  }
  if (strchr(flags, 'E') != NULL && *flag_skip != 1) {
    if (c == 10) {
      printf("$");
    }
  }
  if (strchr(flags, 'T') != NULL && c == '\t') {
    printf("^");
    c = '\t' + 64;
  }
  processing_flag_v(flags, &c);
  *elem_in_str = *elem_in_str + 1;
  if (c == 10) {
    *position = 0;
    *elem_in_str = 0;
  }
  if (*flag_skip == 0) {
    printf("%c", c);
  }
}

void processing_flag_s(char *flags, int c, int *null_str, int *flag_skip,
                       int last_c) {
  if (strchr(flags, 's') != NULL) {
    if (c != 10) {
      *null_str = 0;
      *flag_skip = 0;
    }
    if (c == 10 && last_c == 10) {
      *null_str = *null_str + 1;
      if (*null_str > 1) {
        *flag_skip = 1;
      }
    }
  }
}

void processing_flag_v(char *flags, int *c) {
  if (strchr(flags, 'v') != NULL && *c >= 0 && *c <= 31 && *c != '\n' &&
      *c != '\t') {
    printf("^");
    *c = *c + 64;
  }
  if (strchr(flags, 'v') != NULL && *c == 127) {
    printf("^");
    *c = *c - 128 + 64;
  }
  if (strchr(flags, 'v') != NULL && *c > 127 && *c < 160) {
    printf("M-^");
    *c = *c - 128 + 64;
  }
}

void parsing_flag(char *flags, int argc, char **argv, int *start_file_name,
                  int *flag_error) {
  int flag = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1 &&
        strncmp((argv)[i], "--", 2) != 0 && flag == 0) {
      (*start_file_name)++;
      add_flag_in_array(flags, argv, flag_error, i);
    } else if (strcmp((argv)[i], "--") == 0) {
      (*start_file_name)++;
      i = argc;
    } else if (strcmp(argv[i], "--number-nonblank") == 0) {
      (*start_file_name)++;
      append_flag(flags, 'b');
    } else if (strcmp(argv[i], "--number") == 0) {
      (*start_file_name)++;
      append_flag(flags, 'n');
    } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
      (*start_file_name)++;
      append_flag(flags, 's');
    } else {
      flag = 1;
    }
  }
}

void add_flag_in_array(char *flags, char **argv, int *flag_error, int i) {
  for (int j = 1; j < (int)strlen(argv[i]); j++) {
    char flag = argv[i][j];
    if (flag == 'e') {
      append_flag(flags, 'E');
      append_flag(flags, 'v');
    } else if (flag == 't') {
      append_flag(flags, 'T');
      append_flag(flags, 'v');
    } else if (flag == 'b' || flag == 'E' || flag == 'n' || flag == 's' ||
               flag == 'T' || flag == 'v') {
      append_flag(flags, flag);
    } else {
      *flag_error = 1;
    }
  }
}

void append_flag(char *flags, char flag) {
  if (strchr(flags, flag) == NULL) {
    char buf[2] = "a";
    buf[0] = flag;
    strcat(flags, buf);
  }
}