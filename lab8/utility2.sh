#!/bin/bash

if(($#==0)); then
	read -p "Dati numarul de executii:" nr
elif(($#==1)); then
	nr=$1
else
	exit
fi

if(($nr <= 0)); then
	echo -e "Numarul de executii trebuie sa fie strict pozitiv!\n"
	exit
fi

program="./program4 "
fisier="sort"

for((i=1;i<=$nr;i++)); do
	program="$program $fisier$i"
done

execute=$program

for((i=2;i<=$nr;i++)); do
	execute="$execute & $program"
done

eval $execute


