# Named pipes (extra)
# Crea due script:
# • produttore.sh: genera numeri casuali e li scrive su una named pipe
# • consumatore.sh: legge dalla named pipe e calcola la media
# • Eseguili in parallelo usando mkfifo
# Hint: bc per calcoli decimali
#!/bin/bash

FIFO="/tmp/numeri_pipe"
mkfifo $FIFO

while true; do
    NUMERO=$((RANDOM % 100))
    echo $NUMERO > $FIFO
    sleep 0.5
done