#!/bin/bash

tmp_file1=$(mktemp)
tmp_file2=$(mktemp)
tmp_file3=$(mktemp)

cp testfile.txt $tmp_file1

flags=(-n -b -s -e -t -T -E )
    echo "----------Короткие флаги (Linux, на MacOs иначе работает -e -t)----------:"
for flag in "${flags[@]}"
do
    ./s21_cat $flag $tmp_file1 > $tmp_file3
    cat $flag $tmp_file1 > $tmp_file2

    diff_result=$(diff $tmp_file3 $tmp_file2)

    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

echo "----------Длинные флаги (Linux)----------:"
for flag in "--number-nonblank" "--squeeze-blank" "--number"; do
    ./s21_cat $flag $tmp_file1 > $tmp_file3
    cat $flag $tmp_file1 > $tmp_file2
    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

echo "----------Длинные флаги (MacOS)----------:"
for flag in "--number-nonblank" "--squeeze-blank" "--number"; do
    ./s21_cat $flag $tmp_file1 > $tmp_file3
    case $flag in
        "--number-nonblank")
            cat_command="cat -b";;
        "--squeeze-blank")
            cat_command="cat -s";;
        "--number")
            cat_command="cat -n";;
    esac
    $cat_command $tmp_file1 > $tmp_file2
    diff_result=$(diff $tmp_file2 $tmp_file3)
    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

echo "----------Работа с 2 файлами (короткие флаги) ----------:"
tmp_file4=$(mktemp)
cp testfile2.txt $tmp_file4

for flag in "${flags[@]}"
do
    ./s21_cat $flag $tmp_file1 $tmp_file4 > $tmp_file3
    cat $flag $tmp_file1 $tmp_file4 > $tmp_file2

    diff_result=$(diff $tmp_file3 $tmp_file2)

    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

echo "----------Работа с 2 файлами (длиные флаги Linux) ----------:"
for flag in "--number-nonblank" "--squeeze-blank" "--number"; do
    ./s21_cat $flag $tmp_file1 $tmp_file4 > $tmp_file3
    cat $flag $tmp_file1 $tmp_file4 > $tmp_file2
    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

echo "----------Работа с 2 файлами (длиные флаги MacOS) ----------:"
for flag in "--number-nonblank" "--squeeze-blank" "--number"; do
    ./s21_cat $flag $tmp_file1 $tmp_file4 > $tmp_file3
    case $flag in
        "--number-nonblank")
            cat_command="cat -b";;
        "--squeeze-blank")
            cat_command="cat -s";;
        "--number")
            cat_command="cat -n";;
    esac
    $cat_command $tmp_file1 $tmp_file4 > $tmp_file2
    diff_result=$(diff $tmp_file2 $tmp_file3)
    if [ -z "$diff_result" ]; then
        echo "Тест с флагом $flag прошел успешно."
    else
        echo "Тест с флагом $flag не прошел. Различия:"
        echo "$diff_result"
    fi
done

rm $tmp_file1 $tmp_file2 $tmp_file3 $tmp_file4
