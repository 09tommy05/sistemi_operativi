#!/bin/bash
#ESERCIZIO 1.1
ls $DATA && echo "T" || echo "F"

#ESERCIZIO 1.2
([ -f $DATA ] && echo "File") || ([ -d $DATA ] && echo "Cartella") || (echo "?")

#ESERCIZIO 1.3
[ -f $DATA ] && echo $(( $(cat $DATA) )) || echo "?"