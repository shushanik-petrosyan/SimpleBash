#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int main() {
    char *pattern = NULL;
    char *text = NULL;
    size_t pattern_size = 0;
    size_t text_size = 0;

    printf("Введите регулярное выражение: ");
    getline(&pattern, &pattern_size, stdin);

    printf("Введите текст: ");
    getline(&text, &text_size, stdin);

    // убираем символ переноса строки в конце строк
    pattern[strcspn(pattern, "\n")] = '\0';
    text[strcspn(text, "\n")] = '\0';

    regex_t regex;
    int reti;

    // компилируем регулярное выражение
    reti = regcomp(&regex, pattern, 0);
    if (reti) {
        printf("Ошибка при компиляции регулярного выражения\n");
        return 1;
    }

    // ищем совпадения в тексте
    reti = regexec(&regex, text, 0, NULL, 0);
    if (!reti) {
        printf("+\n"); // совпадение найдено
    } else if (reti == REG_NOMATCH) {
        printf("-\n"); // совпадение не найдено
    } else {
        printf("Ошибка при выполнении регулярного выражения\n");
        return 1;
    }

    // освобождаем память, занятую регулярным выражением и введенными строками
    regfree(&regex);
    free(pattern);
    free(text);

    return 0;
}