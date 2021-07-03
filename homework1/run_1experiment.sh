#!/bin/bash

if [ $# -ne 2 ] ; then 1>&2 echo "Eroare, nu ati apelat scriptul $0 cu cei 2 parametri necesari!" ; exit 128 ; fi

GP=$1        #numarul de instante
datafile=$2  #fisierul cu secventa de sortat

for ((i = 1; i <= $GP; i++))
do
    ./parallel-with-locks $datafile &
done
wait
