#!/bin/bash

if(($#==0)); then
	read -p "[nr executii][executabil][fdate][finstructiuni]" nr exec date inst
	if [[ $exec == "" ]] ; then
		exec="program1"
		date="depozit.bin"
		inst="instr"
	fi
elif(($#==4)); then
	nr=$1
	exec=$2
	date=$3
	inst=$4
elif(($#==1)); then
	exec="program1"
	date="depozit.bin"
	inst="instr"
	nr=$1
else
	exit
fi

if(($nr==0)); then
	exit
else
	format_instr=".dat"
	ext="1$format_instr"
	program="./$exec $date $inst$ext"
	for((i=2;i<=$nr;i++)); do
		ext="$i$format_instr"
		program="$program & ./$exec $date $inst$ext"
	done
	eval $program
fi
