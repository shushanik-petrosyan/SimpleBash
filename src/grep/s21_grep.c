#include <getopt.h>   // парсер
#include <regex.h>    // для регулярных выражений
#include <stdbool.h>  // для булевых значений
#include <stdio.h>    // для скана и принта и гетлайна
#include <stdlib.h>   // для маллоков
#include <string.h>   // на будущее
#define _GNU_SOURCE
#define  _POSIX_C_SOURCE 200809L

//исправить .

typedef struct options {
  int e;  // задает шаблон ()
  int i;  // игнор регистра *
  int v;  // инверсивный поиск *
  int c;  // пишет сколько раз в каждом файле встретилось слово *
  int l;  // ищет среди файлов в каком есть и выводит имя файла ()
  int n;  // выводит номер строки на которой нашел *
  int f;  // работает с первым файлом как с шаблоном ()
  int h;  // выводит совпадающие строки, не предваряя их именами файлов *
  int s;  // подавляет сообщения об ошибках о несуществующих или нечитаемых *
          // файлах *
  int o;  // печатает только совпадающие (непустые) части совпавшей строки ()
} options;

const char *short_options = "e:ivclnf:hso";
struct option long_options[] = {{0, 0, 0, 0}};

bool parser(int argc, char **argv, struct options *options);
void arg_number(int argc, char **argv, struct options *options);
void file_check__flag_s(char **argv, int i, struct options *options,
                        int file_count);
int basic_grep(char **argv, FILE *filename, int i, struct options *options,
               int file_count);
void flag_h(char **argv, int i, struct options *options, int file_count,
            int *print_file_count);
int reg_compilation__flag_i(int reti, regex_t *regex, char *pattern, char *text,
                            struct options *options);
void flag_n(int *file_line_counter, struct options *options, int reti) ;

int main(int argc, char *argv[]) {
  options options = {0};
  parser(argc, argv, &options);
  arg_number(argc, argv, &options);
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
        // printf("\noptarg -- %s\n", optarg);
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
    }
  }
  return check;
}

void arg_number(int argc, char **argv, struct options *options) {
  if (argc <= 2) {
    fprintf(stderr,
            "Usage: grep [OPTION]... PATTERN [FILE]... \n Try 'grep --help' "
            "for more information.");
  } else {
    int file_count = 0;
    for (int i = (optind+1); i < argc; i++) {
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

void file_check__flag_s(char **argv, int i, struct options *options,
                        int file_count) {
  FILE *fp = fopen(argv[i], "r");
  if (fp == NULL) {
    if (!options->s) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
    }
  } else {
    basic_grep(argv, fp, i, options, file_count);
    fclose(fp);
  }
}

int basic_grep(char **argv, FILE *filename, int i, struct options *options,
               int file_count) {
  char *pattern = argv[optind - 1];
  char *text = NULL;
  int pattern_count = 0;
  size_t * size_of = 0;
  int print_file_count = 0;
  int file_line_counter = 0;
  ssize_t read = 0;
  while (getline(&text, &size_of, filename) != -1) {
    regex_t regex;
    int reti;
    regmatch_t matches[1];
    reg_compilation__flag_i(reti, &regex, pattern, text, options);
    reti = regexec(&regex, text, 1, matches, 0);  // скомплированное выражение, текст из файла,
    file_line_counter ++;
    if (!reti) {
      flag_h(argv, i, options, file_count, &print_file_count);
      // printf("%llu %llu\n", matches[0].rm_so, matches[0].rm_eo - 1);
      flag_n(&file_line_counter, options, reti);
      if (options->c) {
        pattern_count++;
      } else if (!options->v) {
        printf("%s", text);
      }
    } else if ((reti == REG_NOMATCH) && (options->v)) {
      flag_h(argv, i, options, file_count, &print_file_count);
      flag_n(&file_line_counter, options, reti);
      printf("%s", text);
    } 
    // else {
    //   fprintf(stderr, "Ошибка при выполнении регулярного выражения\n");
    //   // regfree(&regex);
    //   // free(text);
    //   return 1;
    // }
    regfree(&regex);
  }
  if (options->c && pattern_count!= 0) {
    printf("%d\n", pattern_count);
  } else if (options->c && pattern_count == 0){
    printf("%s:", argv[i]);
    printf("%d\n", pattern_count);
  }
  print_file_count=0;
  free(text);
  return 0;
}

void flag_h(char **argv, int i, struct options *options, int file_count,
            int *print_file_count) {
  if (      ((!options->h) && (file_count > 1) && (!options->c))   || (options->c) && (*print_file_count == 0)           ) {  
    // printf("%s:", argv[i]);
  // } else if ((options->c) && (*print_file_count == 0)){
        printf("%s:", argv[i]);
    // printf("print file count on %d iteration is %d\n", i, *print_file_count);
    *print_file_count = *print_file_count + 1;
    // printf("print file count on %d iteration is %d\n", i, *print_file_count);
  }
  // printf("%d\n", file_count);
}

int reg_compilation__flag_i(int reti, regex_t *regex, char *pattern, char *text,
                            struct options *options) {
  if (options->i) {
    reti = regcomp(regex, pattern, REG_ICASE | REG_EXTENDED);
  } else {
    reti = regcomp(regex, pattern, REG_EXTENDED);
  }
  if (reti) {
    fprintf(stderr, "Ошибка при компиляции регулярного выражения\n");
    regfree(regex);
    free(text);
  }
  return 0;
}

void flag_n(int *file_line_counter, struct options *options, int reti) {
  if (options->n && !reti && !options->v) {
    printf("%d:", *file_line_counter);
  } else if ((reti == REG_NOMATCH) && (options->v) && (options->n)){
    printf("%d:", *file_line_counter);
  }
}