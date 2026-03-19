# Chain di comandi
# Crea una pipeline che:
# Elenchi tutti i processi (ps aux)
# Estragga solo quelli dell’utente corrente
# Ordini per uso di memoria
# Mostri i primi 10
# Hint: sort -k

# -k4 indica il 4 campo (colonna)
ps aux | grep $USER | sort -k4 | head -n 10