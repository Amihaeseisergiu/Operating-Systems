#!/bin/bash

if [ $# -eq 0 ]
then
	read -p "Dati numele fisierului sursa .c : " fisier
else
	fisier=$1
fi

while true
do
	nano $fisier

	read -p "Doriti compilarea programului? (y/n) " ans
	if [ $ans = "y" ]
	then
		output=$(basename $fisier .c)
		gcc $fisier -o $output -Wall
		if [ $? -eq 0 ]
		then
			read -p "Doriti executarea programului? (y/n) " ans
			if [ $ans = "y" ]
			then
				./$output
			fi
			break
		else
			echo "Apasa pentru a continua cu editarea erorilor..."
			read ans
			continue
		fi
	else
		break
	fi
done

