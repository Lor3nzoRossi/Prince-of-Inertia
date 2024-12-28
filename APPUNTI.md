# Unsigned Char e Signed Char

In C, i tipi `unsigned char` e `signed char` sono utilizzati per rappresentare numeri come caratteri, ma in realtà trattano valori numerici:

- **Signed char**: può rappresentare numeri da -128 a 127.
- **Unsigned char**: può rappresentare numeri da 0 a 255.

Questi tipi sono comunemente usati per leggere caratteri come numeri, ad esempio con il formato di input `%hhd` in C.

# Static in C

In C, quando una variabile o una funzione è dichiarata come `static`, essa diventa visibile solo all'interno del file in cui è definita. Questo significa che non può essere utilizzata o acceduta da altri file del programma, garantendo una sorta di "scoping" a livello di file.

# Organizzazione della Memoria

La memoria è divisa sostanzialmente in due parti principali:

- **Stack**: è una parte statica della memoria che contiene tutto ciò che sappiamo sarà allocato di certo (ad esempio, le variabili dichiarate nel codice come un `int`).
- **Heap**: è una parte dinamica della memoria in cui la dimensione degli elementi può cambiare a "runtime", ovvero durante l'esecuzione del programma.

# sizeof

La funzione `sizeof` restituisce la dimensione del tipo di dato da allocare.

# Memoria Dinamica

La libreria `malloc.h` è necessaria per gestire la memoria dinamica in C. Senza questa libreria, le funzioni di allocazione dinamica non funzionerebbero correttamente. Le principali funzioni sono:

- **malloc()**: ad esempio, `(int *) malloc(sizeof(int) * numero)` per allocare memoria per un array di interi.
- **calloc()**: simile a `malloc`, ma inizializza la memoria a zero.
- **realloc()**: modifica uno spazio di memoria precedentemente allocato.
- **free()**: libera la memoria precedentemente allocata.

PUNTATORI GLOBALI

Un puntatore globale in C è un puntatore dichiarato al di fuori di qualsiasi funzione, solitamente all'inizio del programma, che può essere utilizzato da qualsiasi funzione del programma



Fonte: [html.it - Allocazione dinamica della memoria](https://www.html.it/pag/15416/allocazione-dinamica-della-memoria/)
