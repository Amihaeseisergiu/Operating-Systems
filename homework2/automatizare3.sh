#!/bin/bash

if [ $# -ne 3 ] ; then 1>&2 echo "Eroare, nu ati apelat scriptul $0 cu cei 3 parametri necesari!" ; exit 128 ; fi

GP=$1        #numarul maxim de instante
serii=$2     #numarul de serii
N=$3         #secventa maxima de numere

echo -e "Wait for me!!!\n"

touch ~/OutputData/rezultate.csv
echo "Tip suma,GP,N,Fisier,Durata" >> ~/OutputData/rezultate2.csv

for ((j = 10; j <= $N; j=j*10))
do
	for (( k = 1; k <= $serii; k++))
	do
		file="file_"$j"_random-"$k".dat"
		timp=$(./superviser-sequential "$file" 2>&1)

		echo "Suma secventiala,1,$j,$file,$timp" >> ~/OutputData/rezultate2.csv
	done
done

for ((i = 1; i <= $GP; i++))
do
	for ((j = 10; j <= $N; j=j*10))
	do
		for (( k = 1; k <= $serii; k++))
		do
			file="file_"$j"_random-"$k".dat"
			timp=$(./superviser $i "$file" 2>&1)

			echo "Suma paralela,$i,$j,$file,$timp" >> ~/OutputData/rezultate2.csv
		done
	done
done
wait

echo -e "I'm here!\n"
