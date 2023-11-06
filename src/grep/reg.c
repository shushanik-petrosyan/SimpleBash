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
    regmatch_t matches[1]; //В данном примере мы добавили массив matches типа regmatch_t, который используется для хранения информации о совпадениях.

    // Мы также изменили значение второго аргумента функции regexec на 1, чтобы получить информацию только о первом совпадении. 
    // Если совпадение найдено, мы выводим индексы первого совпадения с помощью полей rm_so и rm_eo структуры matches.
    //  Обратите внимание, что индекс последнего символа совпадения равен rm_eo-1, так как rm_eo указывает на индекс следующего символа после совпадения.

    // компилируем регулярное выражение
    reti = regcomp(&regex, pattern, 0);
    if (reti) {
        printf("Ошибка при компиляции регулярного выражения\n");
        return 1;
    }

    // ищем совпадения в тексте
    reti = regexec(&regex, text, 1, matches, 0);
    if (!reti) {
        printf("%d %d\n", matches[0].rm_so, matches[0].rm_eo-1); // выводим индексы первого совпадения
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