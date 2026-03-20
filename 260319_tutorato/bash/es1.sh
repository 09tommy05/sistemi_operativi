#es1 - ok
#Salva l’output di ls -l /etc nel file etc_contents.txt Aggiungi la data corrente alla fine dello stesso file. Salva eventuali errori in errors.log

ls -l /etc 1>./260319_tutorato/bash/etc_contents.txt 2>./260319_tutorato/bash/errors.log && date 1>>./260319_tutorato/bash/etc_contents.txt

#es2
#Crea uno script che: Utilizzando il comando find su /etc per trovare tutti i file che terminano con .conf Salvi i file trovati in found.txt Salvi gli errori (permessi negati) in denied.txt Mostri a video solo il numero totale di file trovati

#nella soluzione usa cat così wc non ritorna anche il nome del file vabbe 
find /etc -name "*.conf" 1>found.txt 2>denied.txt; wc -l found.txt