# Pipeline complessa
# Legga /etc/passwd
# • Estragga tutti gli username (campo 1)
# • Ordini alfabeticamente
# • Numeri le righe
# • Salvi il risultato mantenendo anche output a video con tee
cat /etc/passwd | cut -d ":" -f 1 | sort | nl | tee usernames.txt