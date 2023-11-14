#include "s21_grep.h"

int main(int argc, char *argv[]) {
  options options = {0};


  options.patterns = malloc(argc * sizeof(char *));
  options.file_patterns = malloc(argc * sizeof(char *));


  parser(argc, argv, &options);
  flag_f(&options, argv);
  if (!options.e && !options.f) {
    options.pattern_count = 1;
  }
  arg_number(argc, argv, &options);
  memory_free(&options);
}

bool parser(int argc, char **argv, struct options *options) {
  bool check = true;
  int opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
             -1 &&
         check) {
    switch (opt) {
      case 'e':
        options->e += 1;
        options->patterns[options->pattern_count] = optarg;
        options->pattern_count++;
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'f':
        options->f = 1;
        options->file_patterns[options->file_pattern_count] = optarg;
        options->file_pattern_count++;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        check = false;
        print_error();
    }
  }
  return check;
}

void print_error() {
  fprintf(stderr,
          "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
          "[-C[num]]\n"
          "   [-e pattern] [-f file] [--binary-files=value] [--color=when]\n"
          "   [--context[=num]] [--directories=action] [--label] "
          "[--line-buffered]\n"
          "   [--null] [pattern] [file ...]\n");
}

void arg_number(int argc, char **argv, struct options *options) {
  if (argc <= 2) {
    print_error();
  } else {
    int file_count = 0;
    int f = 0;
    if (options->e) {
      f = 1;
    }
    for (int l = (optind + 1 - f); l < argc; l++) {
      file_count++;
    }
    if (options->e < 1 && options->f != 1) {
      optind++;
    }
    for (int i = optind; i < argc; i++) {
      file_check__flag_s(argv, i, options, file_count);
    }
  }
}

void file_check(char **argv, int i, struct options *options,
                        int file_count) {
  FILE *fp = fopen(argv[i], "r");
  if (fp == NULL) {
    if (!options->s) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
    }
  } else {
    basic_grep(argv, fp, i, options, file_count);
    if (options->v && options->l) {
      printf("%s\n", argv[i]);
    }
  }
  fclose(fp);
}

int basic_grep(char **argv, FILE *filename, int i, struct options *options,
               int file_count) {
  char *text = NULL;
  int founded_pattern_count = 0;
  size_t size_of = 0;
  int print_file_count = 0;
  int file_line_counter = 1;
  ssize_t get = 0;
  while (((get = getline(&text, &size_of, filename))) != -1) {
    int reti;
    regex_t regex;
    regmatch_t matches[1];
    reg_compilation__flag_i(&regex, text, options, argv);
    reti = regexec(&regex, text, 1, matches, 0);
    add_line_break(text);
    if (!reti && !options->v) {
      flag_h(argv, i, options, file_count, &print_file_count);
      flag_n(&file_line_counter, options, reti);
      if (options->c) {
        founded_pattern_count++;
      } else if (!options->v && !options->l) {
        if (options->o) {
          flag_o(reti, matches, regex, text);
        } else {
          printf("%s", text);
        }
      }
    } else if ((reti == REG_NOMATCH) && (options->v) && (!options->l)) {
      flag_h(argv, i, options, file_count, &print_file_count);
      flag_n(&file_line_counter, options, reti);
      printf("%s", text);
    }
    regfree(&regex);
    file_line_counter++;
  }
  flag_c(argv, i, options, file_count, founded_pattern_count, print_file_count);
  print_file_count = 0;
  free(text);
  return 0;
}

int reg_compilation(regex_t *regex, char *text, struct options *options,
                            char **argv) {
  char *pattern;
  if (options->e || options->f) {
    size_t total_length = 0;
    for (int p = 0; p < options->pattern_count; p++) {
      total_length += strlen(options->patterns[p]);
    }

    pattern = malloc((total_length + options->pattern_count - 1) * sizeof(char));
    if (pattern == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    }
    pattern[0] = '\0';
        for (int p = 0; p < options->pattern_count; p++) {
        strcat(pattern, options->patterns[p]);
        if (p < options->pattern_count - 1) {
            strcat(pattern, "|");
        }
    }
} else {
    pattern = strdup(argv[optind - 1]); 
    if (pattern == NULL) {
    fprintf(stderr, "Memory allocation failed\n");} // Use strdup for single pattern
}
  
  if (regcomp(regex, pattern,
              options->i ? (REG_ICASE | REG_EXTENDED) : REG_EXTENDED)) {
    fprintf(stderr, "Error compiling regular expression\n");
    regfree(regex);
    if (pattern!=NULL)
    free(pattern);
    free(text);
  }
  free(pattern);
  return 0;
}

void flag_h(char **argv, int i, struct options *options, int file_count,
            int *print_file_count) {
  if (!(options->c && options->l && file_count == 1)) {
    if (options->l && (*print_file_count == 0) && !options->c) {
      printf("%s\n", argv[i]);
      *print_file_count = *print_file_count + 1;
    } else if ((!options->h) && (file_count > 1) && (!options->c) &&
               (!options->l)) {
      printf("%s:", argv[i]);
      *print_file_count = *print_file_count + 1;
    }
  }
}

void flag_n(int *file_line_counter, struct options *options, int reti) {
  if (!options->l) {
    if (options->n && !reti && !options->v && !options->c) {
      printf("%d:", *file_line_counter);
    } else if ((reti == REG_NOMATCH) && (options->v) && (options->n) &&
               (!options->c)) {
      printf("%d:", *file_line_counter);
    }
  }
}

void flag_f(struct options *options, char **argv) {
  if (options->f) {
    for (int i = 0; i < options->file_pattern_count; i++) {
      FILE *fp = fopen(options->file_patterns[i], "r");
      if (fp == NULL) {
        fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                options->file_patterns[i]);
      } else {
        char *text = NULL;
        size_t size_of = 0;
        ssize_t get = 0;
        while (((get = getline(&text, &size_of, fp))) != -1) {
          if (text[0] == '\n') {
            options->patterns[options->pattern_count] = strdup(".");
          } else {
            text[strcspn(text, "\n")] = '\0';
            options->patterns[options->pattern_count] = strdup(text);
          }
          options->pattern_count++;
          options->patterns = realloc(options->patterns, (options->pattern_count + 1) * sizeof(char *));
          if (options->patterns == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            free(text);
            fclose(fp);
          }
        }
        free(text);
        fclose(fp);
      }
    }
  }
  
}

void flag_c(char **argv, int i, struct options *options, int file_count,
            int founded_pattern_count, int print_file_count) {
  if (options->c && !options->h && print_file_count == 0 && file_count > 1) {
    printf("%s:", argv[i]);
    printf("%d\n", founded_pattern_count);
  } else if (options->c && options->l) {
    if (file_count != 1) {
      printf("%s:", argv[i]);
    }
    if (founded_pattern_count > 0) {
      printf("1\n");
      printf("%s\n", argv[i]);
    } else {
      printf("0\n");
    }

  } else if (options->c && !options->l) {
    printf("%d\n", founded_pattern_count);
  }
}

void flag_o(int reti, regmatch_t *matches, regex_t regex, char *text) {
  char *ptr = text;
  while (!reti && (matches[0].rm_eo != matches[0].rm_so)) {
    printf("%.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so),
           ptr + matches[0].rm_so);
    ptr += matches[0].rm_eo;
    reti = regexec(&regex, ptr, 1, matches, 0);
  }
}

void add_line_break(char *text) {
  if (text[strlen(text) - 1] != '\n') {
    char *enter = "\n";
    strcat(text, enter);
  }
}

void memory_free (struct options *options){
  for (int i = 0; i < options->pattern_count; i++) {
    if (options->patterns[i]!=NULL)
    free(options->patterns[i]);
  }
  if (options->patterns!=NULL)
  free(options->patterns);
  for (int i = 0; i < options->file_pattern_count; i++) {
    if (options->file_patterns[i]!=NULL)
    free(options->file_patterns[i]);
  }
  if (options->file_patterns!=NULL)
  free(options->file_patterns);
}