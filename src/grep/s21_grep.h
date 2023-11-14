#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int f;
  int h;
  int s;
  int o;
  char **patterns;
  int pattern_count;
  char **file_patterns;
  int file_pattern_count;
} options;

const char *short_options = "e:ivclnf:hso";
struct option long_options[] = {{0, 0, 0, 0}};

bool parser(int argc, char **argv, struct options *options);
void arg_number(int argc, char **argv, struct options *options);
void file_check(char **argv, int i, struct options *options,
                        int file_count);
int basic_grep(char **argv, FILE *filename, int i, struct options *options,
               int file_count);
void flag_h(char **argv, int i, struct options *options, int file_count,
            int *print_file_count);
int reg_compilation(regex_t *regex, char *text, struct options *options,
                            char **argv);
void flag_n(int *file_line_counter, struct options *options, int reti);
void print_error();
void flag_f(struct options *options, char **argv);
void flag_c(char **argv, int i, struct options *options, int file_count,
            int founded_pattern_count, int print_file_count);
void flag_o(int reti, regmatch_t *matches, regex_t regex, char *text);
void add_line_break(char *text);
void memory_free (struct options *options);

#endif
