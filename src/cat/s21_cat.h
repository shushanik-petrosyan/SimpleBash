#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef S21_CAT
#define S21_CAT

typedef struct options {
  bool b; // numbers only non-empty lines
  bool e; // but also display end-of-line characters as $
  bool n; // number all output lines
  bool s; // squeeze multiple adjacent blank lines
  bool t; // but also display tabs as ^I
  bool v; // show non-ptinting symbols
} options;

struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"number", no_argument, NULL, 'n'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {0, 0, 0, 0}};

const char *short_options = "beEnstTv";

bool parser(int argc, char **argv, struct options *options);
void file_check(char **str, int i, struct options *options, int *counter,
                char *previous_symbol, char *current_symbol);
void no_option_no_file();
void print_file(FILE *filename, struct options *options, int *counter,
                char *previous_symbol, char *current_symbol);
void arg_number(int argc, char **argv, struct options *options);
void flag_s(FILE *filename, struct options *options, char previous_symbol,
            char current_symbol);
void flag_b_n(struct options *options, char const *previous_symbol,
              char const *current_symbol, int *counter);
void flag_e(struct options *options, char const *current_symbol);
bool flag_t(struct options *options, char const *current_symbol);
bool flag_v(struct options *options, char const *current_symbol);

#endif
