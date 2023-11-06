#include <getopt.h> // парсер
#include <regex.h> // для регулярных выражений
#include <stdbool.h> // для булевых значений
#include <stdio.h> // для скана и принта
#include <stdlib.h> // для маллоков 
#include <string.h> // на будущее

typedef struct options {
  int e; // задает шаблон
  int i; // игнор регистра
  int v; // инверсивный поиск
  int c; // пишет сколько раз в каждом файле встретилось слово
  int l; // ищет среди файлов в каком есть и выводит имя файла
  int n; // выводит номер строки на которой нашел
} options;

const char *short_options = "e:ivcln";

bool parser(int argc, char **argv, struct options *options);
void arg_number(int argc, char **argv);
void file_check(char **str, int i);
// int take_text(int argc, char **argv);
// int take_filename(int argc, char **argv);

int main(int argc, char *argv[]) {
  options options = {0};
  parser(argc, argv, &options);
  // take_text(argc, argv);
  // take_filename(argc, argv);
  printf("%d", options.e);
  printf("%d", options.i);
  printf("%d", options.v);
  printf("%d", options.c);
  printf("%d", options.l);
  printf("%d", options.n);
  arg_number(argc, argv);
}

bool parser(int argc, char **argv, struct options *options) {
  bool check = true;
  int opt = getopt(argc, argv, short_options);
  while (opt != -1 && check) {
    switch (opt) {
    case 'e':
      options->e += 1;
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
    default:
      check = false;
    }
    opt = getopt(argc, argv, short_options);
  }
  return check;
}

void arg_number(int argc, char **argv) {
  if (argc <= 2) {
    fprintf(stdout, "Usage: grep [OPTION]... PATTERN [FILE]... \n Try 'grep --help' for more information.");
  } else {
    for (int i = optind+1; i < argc; i++) {
      file_check(argv, i);
    }
  }
}

  void file_check(char **str, int i) {
  FILE *fp = fopen(str[i], "r");
  if (fp == NULL) {
    fprintf(stdout, "%s: %s: No such file or directory\n", str[0], str[i]);
  } else {
    fprintf(stdout, "%s", str[i]);
    // print_file(fp, options, counter, previous_symbol, current_symbol);
    fclose(fp);
  }
}
