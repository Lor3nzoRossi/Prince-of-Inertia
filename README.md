# Avvio del programma
## comandi

### compilazione del file main (principale) e creazione del file di output main.o
gcc -c main.c -o main.o

### compilazione del file contenente le definizioni delle funzioni e creazione del file di output gamelib.o
gcc -c gamelib.c -o gamelib.o 

### esegue la connessione tra i due file di output
gcc main.o gamelib.o -o game 

### avvio del gioco
game


# ESPANSIONI FUTURE
- Possibilità di generare un numero di stanze variabile con genera_random()
- Possibilità di tirare un dado quando tutti i giocatori sono morti, con la possibilità in caso positivo di respawn di uno dei giocatori
- Possibilità di rianimare un compagno se si torna nella stessa stanza in cui è morto
- Usare sleep per aspettare dove serve