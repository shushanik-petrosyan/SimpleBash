#include "s21_cat.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#define no_argument 0

int main(int argc, char *argv[]) {
  options options = {0};
  if (parser(argc, argv, &options) == 1) {
    arg_number(argc, argv, &options);
  }
  return 0;
}

bool parser(int argc, char **argv, struct options *options) {
  int opt;
  bool flag = true;
  opt = getopt_long(argc, argv, short_options, long_options, NULL);
  while (flag && opt != -1) {
    switch (opt) {
    case 'b':
      options->b = true;
      break;
    case 'n':
      options->n = true;
      break;
    case 'e':
      options->e = true;
      options->v = true;
      break;
    case 's':
      options->s = true;
      break;
    case 't':
      options->t = true;
      options->v = true;
      break;
    case 'T':
      options->t = true;
      break;
    case 'E':
      options->e = true;
      break;
    case '?':
    default:
      flag = false;
      printf("%s: illegal option %c\n usage: %s [-benstuv] [file ...]\n",
             argv[0], optopt, argv[0]);
    }
    opt = getopt_long(argc, argv, short_options, long_options, NULL);
  }
  return flag;
}

void no_option_no_file() {
  char cat_without_file[265];
  while (1) {
    scanf("%264s", cat_without_file);

    printf("%s\n", cat_without_file);
  }
}

void arg_number(int argc, char **argv, struct options *options) {
  if (argc == 1) {
    no_option_no_file();
  } else {
    int counter = 0;
    char previous_symbol = '\n';
    char current_symbol = 0;
    for (int i = optind; i < argc; i++) {
      file_check(argv, i, options, &counter, &previous_symbol, &current_symbol);
    }
  }
}

void file_check(char **str, int i, struct options *options, int *counter,
                char *previous_symbol, char *current_symbol) {
  FILE *fp = fopen(str[i], "r");
  if (fp == NULL) {
    fprintf(stdout, "%s: %s: No such file or directory\n", str[0], str[i]);
  } else {
    print_file(fp, options, counter, previous_symbol, current_symbol);
    fclose(fp);
  }
}
void print_file(FILE *filename, struct options *options, int *counter,
                char *previous_symbol, char *current_symbol) {
  while ((*current_symbol = fgetc(filename)) != EOF) {
    flag_s(filename, options, (*previous_symbol), (*current_symbol));
    flag_b_n(options, previous_symbol, current_symbol, counter);
    flag_e(options, current_symbol);
    if (flag_t(options, current_symbol)) {
      continue;
    }
    if (flag_v(options, current_symbol) == true)
      putc(*current_symbol, stdout);

    *previous_symbol = *current_symbol;
  }
}
void flag_s(FILE *filename, struct options *options, char previous_symbol,
            char current_symbol) {
  if (options->s && previous_symbol == '\n' && current_symbol == '\n') {
    current_symbol = fgetc(filename);
    while (current_symbol == '\n') {
      current_symbol = fgetc(filename);
    }
    ungetc(current_symbol, filename);
  }
}

void flag_b_n(struct options *options, char const *previous_symbol,
              char const *current_symbol, int *counter) {
  if (options->b && *previous_symbol == '\n' && *current_symbol != '\n') {
    fprintf(stdout, "%6d\t", ++(*counter));
  } else if (options->n && *previous_symbol == '\n' && !(options->b)) {
    fprintf(stdout, "%6d\t", ++(*counter));
  }
}

void flag_e(struct options *options, char const *current_symbol) {
  if (options->e && *current_symbol == '\n') {
    fprintf(stdout, "%c", '$');
  }
}

bool flag_t(struct options *options, char const *current_symbol) {
  bool flag = false;
  if (options->t && *current_symbol == '\t') {
    printf("^I");
    flag = true;
  }
  return flag;
}

bool flag_v(struct options *options, char const *current_symbol) {
  unsigned char c = *current_symbol;
  bool flag = true;
  if (options->v) {
    flag = false;
    int asci_dif;
    if (c < 32 && c != 9 && c != 10) {
      asci_dif = 64;
      printf("^%c", c + asci_dif);
    } else if (c == 127) {
      asci_dif = -64;
      printf("^%c", c + asci_dif);
    } else if (c > 127 && c < 160) {
      asci_dif = -64;
      printf("M-^%c", c + asci_dif);
    } else if (c >= 160) {
      asci_dif = -128;
      printf("M-%c", c + asci_dif);
    } else {
      putchar(c);
    }
  }
  return flag;
}
