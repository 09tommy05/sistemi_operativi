#!/bin/bash
# Parsing argomenti con shift
# Scrivi uno script saluta.sh che accetti un numero variabile di nomi come
# argomenti e stampi un saluto per ciascuno usando shift.
while [[ $1 != "" ]]; do
    echo "Ciao $1"
    shift
done