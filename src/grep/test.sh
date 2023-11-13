#!/bin/bash

tmp_file1=$(mktemp)
tmp_file2=$(mktemp)
tmp_file3=$(mktemp)
tmp_file4=$(mktemp)

cp testfile.txt $tmp_file1
cp testfile2.txt $tmp_file4

flags=(
"-v"
"-c" 
"-l" 
"-n" 
"-h"
"-o" 
"-s" 
"-v"
"-c"
"-v -l"
"-v -n"
"-v -h"
"-v -o"
"-v -s"
"-c -l"
"-c -n"
"-c -h"
"-c -o"
"-c -s"
"-l -n"
"-l -h"
"-l -o"
"-l -s"
"-n -h"
"-n -o"
"-n -s"
"-h -o"
"-h -s"
"-o -s")
search=(
    "old"
    "poor"
    "a..a"
    "world"
    "int"
    "here"
    "mama"
    "aboba"
    "hi"
    "as"
    "a"
)
echo "-------1 file-------"
for flag in "${flags[@]}"; do
    for search_term in "${search[@]}"; do
        ./s21_grep "$flag" "$search_term" "$tmp_file1" > "$tmp_file3"
        grep "$flag" "$search_term" "$tmp_file1" > "$tmp_file2"
        let "COUNTER++"
        diff_result=$(diff "$tmp_file3" "$tmp_file2")

        if [ -z "$diff_result" ]; then
            let "SUCCESS++"
            echo "Тест с флагом $flag и $search_term прошел успешно.(1 файл)"
        else
            echo "Тест с флагом $flag и $search_term не прошел. (1 файл) Различия:"
            echo "$diff_result"
        fi
    done
done

echo "-------2 files-------"
for flag in "${flags[@]}"; do
    for search_term in "${search[@]}"; do
        ./s21_grep "$flag" "$search_term" "$tmp_file1" "$tmp_file4" > "$tmp_file3"
        grep "$flag" "$search_term" "$tmp_file1" "$tmp_file4" > "$tmp_file2"
        let "COUNTER++"
        diff_result=$(diff "$tmp_file3" "$tmp_file2")

        if [ -z "$diff_result" ]; then
            echo "Тест с флагом $flag и $search_term прошел успешно.(2 файла)"
            let "SUCCESS++"
        else
            echo "Тест с флагом $flag и $search_term не прошел (2 файла). Различия:"
            echo "$diff_result"
        fi
    done
done

echo "-------with 1 and 2 files but flag(s) at the end-------"
for flag in "${flags[@]}"; do
    for search_term in "${search[@]}"; do
        ./s21_grep "$search_term" "$tmp_file1" > "$tmp_file3" "$flag"
        grep "$search_term" "$tmp_file1" > "$tmp_file2" "$flag"
        let "COUNTER++"
        diff_result=$(diff "$tmp_file3" "$tmp_file2")
        if [ -z "$diff_result" ]; then
            echo "Тест с флагом $flag и $search_term прошел успешно.(1 файл)"
            let "SUCCESS++"
        else
            echo "Тест с флагом $flag и $search_term не прошел. (1 файл) Различия:"
            echo "$diff_result"
        fi
    done
done

for flag in "${flags[@]}"; do
    for search_term in "${search[@]}"; do
        ./s21_grep  "$search_term" "$tmp_file1" "$tmp_file4" > "$tmp_file3" "$flag"
        grep  "$search_term" "$tmp_file1" "$tmp_file4" > "$tmp_file2" "$flag"
        let "COUNTER++"
        diff_result=$(diff "$tmp_file3" "$tmp_file2")

        if [ -z "$diff_result" ]; then
            echo "Тест с флагом $flag и $search_term прошел успешно.(2 файла)"
            let "SUCCESS++"
        else
            echo "Тест с флагом $flag и $search_term не прошел (2 файла). Различия:"
            echo "$diff_result"
        fi
    done
done

echo "-----------flag f and combinations------"


printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"


# Удаляем временные файлы
rm "$tmp_file1" "$tmp_file2" "$tmp_file3" "$tmp_file4"