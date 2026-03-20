# Named pipes (extra)
# Crea due script:
# • produttore.sh: genera numeri casuali e li scrive su una named pipe
# • consumatore.sh: legge dalla named pipe e calcola la media
# • Eseguili in parallelo usando mkfifo
# Hint: bc per calcoli decimali
#!/bin/bash

FIFO="/tmp/numeri_pipe"
cat $FIFO

SUM=0
COUNT=0
while true; do
    if read NUMERO < $FIFO; then
        SUM=$((SUM + NUMERO))
        COUNT=$((COUNT + 1))
        MEDIA=$(echo "scale=2; $SUM / $COUNT" | bc)
        echo "Media attuale: $MEDIA"
    fi
done