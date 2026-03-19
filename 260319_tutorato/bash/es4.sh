# Combinazione redirezioni
# Legga nomi da input.txt
# Per ogni nome, cerchi file corrispondenti con find in /
# Salvi risultati in results.txt ed errori in errors.txt
# Rediriga sia stdout che stderr su complete.log
if [ ! -f input.txt ]; then #RICORDATI I MALEDETTI SPAZI CON LE QUADRE
    echo "input.txt non esiste."
    exit
fi

files=( $(cat input.txt) );
for i in ${!files[@]}; do
    find /home/tommy/ -name ${files[$i]} &>>complete.log
done