#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
  options options = {0};
  parser(argc, argv, &options);
  printf("%d", options.e);
  printf("%d", options.i);
  printf("%d", options.v);
  printf("%d", options.c);
  printf("%d", options.l);
  printf("%d", options.n);
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
