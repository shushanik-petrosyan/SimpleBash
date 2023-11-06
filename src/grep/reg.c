#include <stdio.h>
#include <regex.h>

int main() {

    
    char pattern[] = "ab";
    char text[] = "abcabcd a b ababcdef bac";

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

    // освобождаем память, занятую регулярным выражением
    regfree(&regex);

    return 0;
}