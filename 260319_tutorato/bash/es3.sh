#Crea uno script che:
#• Crea un file numeri.txt (se non esiste) con numeri da 1 a 100 (uno per
#riga), poi:
#• Usa wc per contare le righe
#• Usa sort per disordinarli su random.txt

if [ -f numeri.txt ]; then
    echo "numeri.txt esiste"
    exit
fi

for i in {1..100}; do
    echo $i >> numeri.txt;
done

wc -l numeri.txt
sort -R numeri.txt > random.txt