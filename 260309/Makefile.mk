CC=gcc
CFLAGS=-Wall -Wextra

app: main.o utils.o
	$(CC) $(CFLAGS) $^ -o $@ #$^ --> espande le dipendenze del target app (main.o utils.o), $@ espande il nome del target (app)

main.o: main.c utils.h
	$(CC) $(CFLAGS) -c $< -o $@ #$< stampa solo la prima dipendenza (main.c)

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c $< -o $@