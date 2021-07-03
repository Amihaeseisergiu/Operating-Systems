#!/bin/bash

if [ $# -ne 3 ] ; then 1>&2 echo "Eroare, nu ati apelat scriptul $0 cu cei 3 parametri necesari!" ; exit 128 ; fi

GP=$1        #numarul maxim de instante
serii=$2     #numarul de serii
N=$3         #secventa maxima de numere

touch ~/OutputData/rezultate.csv
echo "Tip sortare,GP,N,Fisier,Durata" >> ~/OutputData/rezultate.csv

for ((i = 7; i <= $GP; i++))
do
	for ((j = 10; j <= $N; j=j*10))
	do
		file="file_"$j"_best-case.dat"
		cp $file "tmplab8sup.dat"
		timp=$(./run_1experiment $i "tmplab8sup.dat" 2>&1)
		cp $file "tmplab8sup.dat"
		timp2=$(./run_2experiment "tmplab8sup.dat" 2>&1)
		rm "tmplab8sup.dat"

		echo "Sortare paralela,$i,$j,$file,$timp" >> ~/OutputData/rezultate.csv
		echo "Sortare secventiala,$i,$j,$file,$timp2" >> ~/OutputData/rezultate.csv

		for (( k = 1; k <= $serii; k++))
		do
			file="file_"$j"_random-"$k".dat"
			cp $file "tmplab8sup.dat"
			timp=$(./run_1experiment $i "tmplab8sup.dat" 2>&1)
			cp $file "tmplab8sup.dat"
			timp2=$(./run_2experiment "tmplab8sup.dat" 2>&1)
			rm "tmplab8sup.dat"

			echo "Sortare paralela,$i,$j,$file,$timp" >> ~/OutputData/rezultate.csv
			echo "Sortare secventiala,$i,$j,$file,$timp2" >> ~/OutputData/rezultate.csv
		done

		file="file_"$j"_worst-case.dat"
		cp $file "tmplab8sup.dat"
		timp=$(./run_1experiment $i "tmplab8sup.dat" 2>&1)
		cp $file "tmplab8sup.dat"
		timp2=$(./run_2experiment "tmplab8sup.dat" 2>&1)

		rm "tmplab8sup.dat"
		echo "Sortare paralela,$i,$j,$file,$timp" >> ~/OutputData/rezultate.csv
		echo "Sortare secventiala,$i,$j,$file,$timp2" >> ~/OutputData/rezultate.csv
	done
done
wait
