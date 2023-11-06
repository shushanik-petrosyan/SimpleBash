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
./s21_cat --number-nonblank $tmp_file1 > $tmp_file3
cat --number-nonblank $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом number-nonblank прошел успешно."
else
    echo "Тест с флагом number-nonblank не прошел. Различия:"
    echo "$diff_result"
fi

./s21_cat --squeeze-blank $tmp_file1 > $tmp_file3
cat --squeeze-blank $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом squeeze-blank прошел успешно."
else
    echo "Тест с флагом squeeze-blank не прошел. Различия:"
    echo "$diff_result"
fi

./s21_cat --number $tmp_file1 > $tmp_file3
cat --number $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом number прошел успешно."
else
    echo "Тест с флагом number не прошел. Различия:"
    echo "$diff_result"
fi

echo "----------Длинные флаги (MacOS)----------:"
./s21_cat --number-nonblank $tmp_file1 > $tmp_file3
cat -b $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом number-nonblank прошел успешно."
else
    echo "Тест с флагом number-nonblank не прошел. Различия:"
    echo "$diff_result"
fi

./s21_cat --squeeze-blank $tmp_file1 > $tmp_file3
cat -s $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом squeeze-blank прошел успешно."
else
    echo "Тест с флагом squeeze-blank не прошел. Различия:"
    echo "$diff_result"
fi

./s21_cat --number $tmp_file1 > $tmp_file3
cat -n $tmp_file1 > $tmp_file2
if [ -z "$diff_result" ]; then
    echo "Тест с флагом number прошел успешно."
else
    echo "Тест с флагом number не прошел. Различия:"
    echo "$diff_result"
fi

echo "----------Работа с 2 файлами (короткие и длинные флаги)----------:"
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







    


# Удаляем временные файлы
rm $tmp_file1 $tmp_file2
