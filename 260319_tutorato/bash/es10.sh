#!/bin/bash

#Somma di argomenti
#Scrivi uno script somma.sh che accetti esattamente due numeri come
#argomenti e ne stampi la somma.
#Se il numero di argomenti è diverso da 2, lo script deve stampare un
#messaggio di errore su stderr e terminare con exit status 1.
if [ $# -ne 2 ]; then
    echo "Devi mettere esattamente 2 argomenti!" >&2
    exit 1
fi

s=$(($1+$2))

echo "Somma= $s"