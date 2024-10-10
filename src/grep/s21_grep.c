#include "s21_grep.h"

int main(int argc, char **argv) {
  FLAGS flag = {0};
  int code_error = 0;
  int flag_have = 0;
  int filename[10];
  int flag_error = 0;
  int fileCount = 0;
  int pattern[32];
  int number_pattern = 0;
  for (int i = 0; i < 32; i++) {
    pattern[i] = 0;
  }
  flag_have = checking_flag_e(argc, argv);
  parsing(pattern, &number_pattern, &flag, argc, argv, &flag_error, filename,
          &fileCount, &flag_have);
  if (flag_error == 0) {
    for (int i = 0; i < fileCount; i++) {
      search(argv, pattern, number_pattern, filename[i], fileCount, flag,
            &flag_error);
    }
    if (flag_error == 2) {
      code_error = 2;
    }
  } else {
    printf("flag error: no such flag exists");
  }
  return code_error;
}

void search(char **argv, int pattern[], int number_pattern, int n, int fileCount,
           FLAGS flags, int *flag_error) {
  char *filename = argv[n];
  char *line = NULL;
  int number_of_matches = 0;
  int options = REG_EXTENDED;
  if (flags.flag_i == 1) {
    options |= REG_ICASE;
  }
  size_t line_length = 0;
  FILE *f;
  f = fopen(filename, "r");
  if (f != NULL) {
    int num_str = 0;
    while (getline(&line, &line_length, f) != -1) {
      num_str++;
      search_help(number_pattern, argv, pattern, options, line, fileCount, flags,
                 num_str, filename, &number_of_matches);
    }
    if ((fileCount == 1 || flags.flag_h == 1) && flags.flag_c == 1) {
      if (flags.flag_l != 0 && number_of_matches > 1) {
        number_of_matches = 1;
      }
      printf("%d\n", number_of_matches);
    } else if (fileCount > 1 && flags.flag_c == 1) {
      if (flags.flag_l != 0 && number_of_matches > 1) {
        number_of_matches = 1;
      }
      printf("%s:%d\n", filename, number_of_matches);
    }
    if (flags.flag_l == 1 && number_of_matches != 0) {
      printf("%s\n", filename);
    }
    free(line);
  } else if (flags.flag_s == 0) {
    printf("grep: %s: No such file or directory\n", filename);
    *flag_error = 2;
  }
}

void search_help(int number_pattern, char **argv, int pattern[], int options,
                char *line, int fileCount, FLAGS flags, int num_str,
                char *filename, int *number_of_matches) {
  regex_t regex;
  // флаг нужен для того случая когда несколько паттернов и есть инверсия
  int flag_inversion = 0;
  if (flags.flag_f != 0) {
    number_pattern++;
  }
  for (int i = 0; i < number_pattern; i++) {
    int ret = 0;
    if (i == number_pattern - 1 && flags.flag_f != 0) {
      FILE *pat_fp = fopen(argv[flags.flag_f], "r");
      if (pat_fp != NULL) {
        char patf[256];
        fgets(patf, 256, pat_fp);
        fclose(pat_fp);
        patf[strcspn(patf, "\n")] = '\0';
        ret = regcomp(&regex, patf, options);
      } else {
        printf("grep: %s: No such file or directory\n", argv[flags.flag_f]);
      }
    } else {
      ret = regcomp(&regex, argv[pattern[i]], options);
    }
    if (ret != 0) {
      printf("error regex");
    }
    ret = regexec(&regex, line, 0, NULL, 0);
    if (ret == 0 && flags.flag_v == 1) {
      flag_inversion = 1;
      i = number_pattern;
    }
    search_help_help(ret, fileCount, number_pattern, flags, &i, line, num_str,
                    filename, flag_inversion, number_of_matches);
    regfree(&regex);
  }
}

void search_help_help(int ret, int fileCount, int number_pattern, FLAGS flags, int *i,
                     char *line, int num_str, char *filename, int flag_inversion,
                     int *number_of_matches) {
  if (ret == 0 && (fileCount == 1 || flags.flag_h == 1) && flags.flag_v == 0 &&
      flags.flag_c == 0 && flags.flag_l == 0) {
    checking_for_two_pattern(number_pattern, i);
    print_string_one_file(flags.flag_n, line, num_str);
    checking_end_string(line);
    // Файлов много совпадения найдены  нет иневрсии вывести совпадения
  } else if (ret == 0 && fileCount > 1 && flags.flag_v == 0 &&
             flags.flag_c == 0 && flags.flag_l == 0 && flags.flag_h == 0) {
    print_string_much_file(flags.flag_n, filename, line, num_str);
    checking_end_string(line);
    checking_for_two_pattern(number_pattern, i);
    // Файл 1 совпадения ненайдены  иневрсии нет вывести совпадения
  } else if (ret != 0 && (fileCount == 1 || flags.flag_h == 1) &&
             flags.flag_v != 0 && flags.flag_c == 0 && flags.flag_l == 0 &&
             *i == number_pattern - 1 && flag_inversion == 0) {
    print_string_one_file(flags.flag_n, line, num_str);
    checking_end_string(line);
    checking_for_two_pattern(number_pattern, i);
  } else if (ret != 0 && fileCount > 1 && flags.flag_v != 0 &&
             flags.flag_c == 0 && flags.flag_l == 0 && *i == number_pattern - 1 &&
             flag_inversion == 0 && flags.flag_h == 0) {
    print_string_much_file(flags.flag_n, filename, line, num_str);
    checking_end_string(line);
    checking_for_two_pattern(number_pattern, i);
  } else if (ret == 0 && flags.flag_v == 0 &&
             (flags.flag_c == 1 || flags.flag_l == 1)) {
    *number_of_matches = *number_of_matches + 1;
    checking_for_two_pattern(number_pattern, i);
  } else if (ret != 0 && flags.flag_v == 1 &&
             (flags.flag_c == 1 || flags.flag_l == 1) && *i == number_pattern - 1 &&
             flag_inversion == 0) {
    *number_of_matches = *number_of_matches + 1;
    checking_for_two_pattern(number_pattern, i);
  }
}

void checking_for_two_pattern(int number_pattern, int *i) {
  if (number_pattern > 1) {
    *i = number_pattern;
  }
}

void print_string_one_file(int flag_num, char *line, int num_str) {
  if (flag_num == 0) {
    printf("%s", line);
  } else {
    printf("%d:%s", num_str, line);
  }
}

void print_string_much_file(int flag_num, char *filename, char *line,
                           int num_str) {
  if (flag_num == 0) {
    printf("%s:%s", filename, line);
  } else {
    printf("%s:%d:%s", filename, num_str, line);
  }
}

void parsing(int pattern[], int *number_pattern, FLAGS *flags, int argc, char **argv,
             int *flag_error, int *filename, int *fileCount, int *flag_have) {
  int flag_have_e = 0;
  for (int i = 1; i < argc; i++) {
    const char *arg = argv[i];
    if (arg[0] == '-' && strlen(arg) == 2) {
      help_parsing_par_flag(arg[1], flags, &flag_have_e, flag_have, argc, pattern,
                            number_pattern, &i, flag_error);
    } else if (arg[0] == '-' && strlen(arg) == 3) {
      help_parsing_par_flag(arg[1], flags, &flag_have_e, flag_have, argc, pattern,
                            number_pattern, &i, flag_error);
      help_parsing_par_flag(arg[2], flags, &flag_have_e, flag_have, argc, pattern,
                            number_pattern, &i, flag_error);

    } else {
      if (flag_have_e == 1 || *flag_have == 1 || flags->flag_f != 0) {
        filename[*fileCount] = i;
        *fileCount = *fileCount + 1;
      } else {
        pattern[*number_pattern] = i;
        *number_pattern = *number_pattern + 1;
        flag_have_e = 1;
      }
    }
  }
}

void help_parsing_par_flag(char fl, FLAGS *flags, int *flag_have_e,
                           int *flag_have, int argc, int pattern[],
                           int *number_pattern, int *i, int *flag_error) {
  if (fl == 'e') {
    *flag_have_e = 1;
    *flag_have = 0;
    if (*i + 1 < argc) {
      pattern[*number_pattern] = *i + 1;
      *number_pattern = *number_pattern + 1;
      *i = *i + 1;
    } else {
      printf("Missing argument for -e flag\n");
      *flag_error = 1;
    }
  } else if (fl == 'i') {
    flags->flag_i = 1;
  } else if (fl == 'v') {
    flags->flag_v = 1;
  } else if (fl == 'c') {
    flags->flag_c = 1;
  } else if (fl == 'l') {
    flags->flag_l = 1;
  } else if (fl == 'n') {
    flags->flag_n = 1;
  } else if (fl == 'h') {
    flags->flag_h = 1;
  } else if (fl == 's') {
    flags->flag_s = 1;
  } else if (fl == 'f') {
    flags->flag_f = *i + 1;
    *i = *i + 1;
  } else if (fl == 'o') {
    flags->flag_o = 1;
  }
}

void checking_end_string(char *line) {
  if (line[strlen(line) - 1] != 10) {
    printf("\n");
  }
}

int checking_flag_e(int argc, char **argv) {
  int flag = 0;
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-e") == 0) {
      flag = 1;
    }
  }
  return flag;
}
