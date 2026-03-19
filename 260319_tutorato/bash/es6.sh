# Elaborazione di testo
# Crea un comando che:
# • Dati dei files con estensione .log
# • Estragga le righe contententi la stringa ERROR
# • Filtri i tre errori più frequenti
# • Li scriva in stdout
# Formato errori: “ERROR:<error message>”
# Comandi utilizzati: cat cut grep head sort uniq

cat *.log | grep "ERROR" | sort | uniq -c | sort -r | head -n 3