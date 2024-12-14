/*Questo file contiene le definizioni delle funzioni che implementano il gioco.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamelib.h"

/**
 * Controlla se l'input da tastiera è un numero 
 * e continua a richiedere l'input fino a condizione soddisfatta
 */
int leggi_numero(const char* messaggio) {
    int num;
    printf(messaggio );
    while (1) {
        if (scanf("%d", &num) == 1) { //Se l'input e' valido
            return num;
        } else {
            printf("Input non valido. Inserisci un numero: ");
            while (getchar() != '\n'); //pulizia del buffer di input
        }
    }
}

void menu_stanze(){
    int scelta;
    do {
        // Stampa del menu
        printf("\n==== MENU GESTIONE STANZE ====");
        printf("\n1. Inserisci una nuova stanza");
        printf("\n2. Cancella una stanza");
        printf("\n3. Stampa tutte le stanze");
        printf("\n4. Genera stanza casuale");
        printf("\n5. Chiudi la mappa e termina");
        printf("\nSeleziona un'opzione (1-5): ");

        // Lettura dell'opzione scelta dall'utente
        while (scanf("%d", &scelta) != 1 || scelta < 1 || scelta > 5) {
            printf("\nInput non valido! Inserisci un numero tra 1 e 5: ");
            while (getchar() != '\n'); // Pulizia del buffer
        }

        // Gestione delle opzioni
        switch (scelta) {
            case 1:
                // ins_stanza();
                break;
            case 2:
                // canc_stanza();
                break;
            case 3:
                // stampa_stanze();
                break;
            case 4:
                // genera_random();
                break;
            case 5:
                // chiudi_mappa();
                printf("\nMappa chiusa. Programma terminato.\n");
                break;
            default:
                printf("\nErrore inaspettato.\n");
        }

    } while (scelta != 5);
}


/**
 * Inizializza un giocatore
 * @param giocatore struct giocatore da inzializzare
 * @param isPrincipe indica se e' stato scelto un principe (-1 se non e' stato ancora scelto e 0 se e' stato scelto)
 * @param numGiocatore numero della creazione del giocatore
 */
void inizializza_giocatore(struct Giocatore* giocatore, int* isPrincipe, int numGiocatore) {
    numGiocatore += 1; // Numero del giocatore per la visualizzazione

    // Non è ancora stato scelto principe
    if (*isPrincipe == -1) {
        char* stringaClasse = "Inserisci 0 per principe o 1 per doppleganger: ", numGiocatore;
        giocatore->classe_giocatore = leggi_numero(stringaClasse);
        if (giocatore->classe_giocatore == 0) { // Se l'utente sceglie principe
            *isPrincipe = 0;  // Aggiornamento della variabile sulla presenza del principe
        }
    } else if (*isPrincipe == 0) { // È stato scelto principe
        printf("Visto che e' stato scelto un principe, e' stata automaticamente impostata la classe doppleganger.\n");
        giocatore->classe_giocatore = 1;  // Impostazione automatica a doppleganger
    }

    // Inserimento dei punti vita massimi
    giocatore->p_vita_max = leggi_numero("Inserisci i punti vita massimi per il giocatore: ");
    giocatore->p_vita = giocatore->p_vita_max;  // Imposta i punti vita correnti uguali a quelli massimi

    // Inserimento dei dadi per l'attacco
    giocatore->dadi_attacco = leggi_numero("Inserisci il numero di dadi attacco per il giocatore: ");

    // Inserimento dei dadi per la difesa
    giocatore->dadi_difesa = leggi_numero("Inserisci il numero di dadi difesa per il giocatore: ");

    giocatore->posizione = NULL; // Inizializzazione della posizione (di default NULL)
}


// Funzione per aggiungere una stanza
static void ins_stanza() {
    struct Stanza* nuovaStanza = (struct Stanza*)malloc(sizeof(struct Stanza));
    //controllo se spazio in memoria è sufficiente per la creazione della stanza
    if (!nuovaStanza) {
        printf("Errore: memoria insufficiente.\n");
        return;
    }

    //inserimento parametri della nuova stanza
    nuovaStanza->tipo_stanza = leggi_numero("Inserisci il tipo della stanza (numero intero): ");
    nuovaStanza->trabocchetto = leggi_numero("Inserisci il tipo di trabocchetto (numero intero): ");
    nuovaStanza->tesoro = leggi_numero("Inserisci il tipo di tesoro (numero intero): ");

    //nel caso non fosse stata ancora aggiunta nessuna stanza
    if (pFirst == NULL) {
        pFirst = nuovaStanza;
        pUltima = nuovaStanza;
        printf("Prima stanza aggiunta con successo!\n");
        return;
    }

    //nel caso esistesse già una stanza nella lista, chiedere dove creare questa nuova stanza
    printf("In quale direzione vuoi aggiungere la stanza? (sopra/sotto/sinistra/destra): ");
    char direzione[10];
    scanf("%s", direzione);

    //inserimento della nuova stanza nella direzione scelta
    if (strcmp(direzione, "sopra") == 0) {
        if (pUltima->stanza_sopra != NULL) {
            printf("Errore: esiste gia' una stanza sopra.\n");
        } else {
            pUltima->stanza_sopra = nuovaStanza;
            printf("Stanza aggiunta sopra con successo!\n");
        }
    } else if (strcmp(direzione, "sotto") == 0) {
        if (pUltima->stanza_sotto != NULL) {
            printf("Errore: esiste gia' una stanza sotto.\n");
        } else {
            pUltima->stanza_sotto = nuovaStanza;
            printf("Stanza aggiunta sotto con successo!\n");
        }
    } else if (strcmp(direzione, "sinistra") == 0) {
        if (pUltima->stanza_sinistra != NULL) {
            printf("Errore: esiste gia' una stanza a sinistra.\n");
        } else {
            pUltima->stanza_sinistra = nuovaStanza;
            printf("Stanza aggiunta a sinistra con successo!\n");
        }    
    } else if (strcmp(direzione, "destra") == 0) {
        if (pUltima->stanza_destra != NULL) {
            printf("Errore: esiste gia' una stanza a destra.\n");
        } else {
            pUltima->stanza_destra = nuovaStanza;
            printf("Stanza aggiunta a destra con successo!\n");
        }
    } else {
        printf("Direzione non valida. Stanza non aggiunta.\n");
        free(nuovaStanza); //liberazione della memoria riservata per la nuova stanza che però è impossibile da creare
        return;
    }
    
    pUltima = nuovaStanza; //impostare la stanza appena creata come l'ultima
}

/**
 * Elimina l'ultima stanza
 */
static void canc_stanza(struct Stanza* stanza_corrente, char direzione) {
    //controllo se esistono stanze
    if (pFirst == NULL) {
        printf("La mappa è vuota. Impossibile cancellare una stanza.\n");
        return;
    }

    //controllo se esiste solo una stanza
    if (
        pFirst->stanza_destra == NULL && pFirst->stanza_sinistra == NULL &&
        pFirst->stanza_sopra == NULL && pFirst->stanza_sotto == NULL
    ) {
        free(pFirst);
        pFirst = NULL;
        printf("L'ultima stanza è stata cancellata.\n");
        return;
    }
}

// Funzione per stampare le informazioni di una stanza
static void stampa_stanza(struct Stanza* stanza) {
    if (stanza == NULL) {
        printf("Stanza vuota.\n");
        return;
    }

    printf("Tipo stanza: %d, Trabocchetto: %d, Tesoro: %d\n", stanza->tipo_stanza, stanza->trabocchetto, stanza->tesoro);
}

// Funzione per stampare tutte le stanze collegate
static void stampa_stanze(struct Stanza* stanza_iniziale) {
    if (stanza_iniziale == NULL) {
        printf("Mappa vuota.\n");
        return;
    }

    printf("Stanza iniziale:\n");
    stampa_stanza(stanza_iniziale);

    if (stanza_iniziale->stanza_destra) {
        printf("Stanza destra:\n");
        stampa_stanza(stanza_iniziale->stanza_destra);
    }
    if (stanza_iniziale->stanza_sinistra) {
        printf("Stanza sinistra:\n");
        stampa_stanza(stanza_iniziale->stanza_sinistra);
    }
    if (stanza_iniziale->stanza_sopra) {
        printf("Stanza sopra:\n");
        stampa_stanza(stanza_iniziale->stanza_sopra);
    }
    if (stanza_iniziale->stanza_sotto) {
        printf("Stanza sotto:\n");
        stampa_stanza(stanza_iniziale->stanza_sotto);
    }
}

// Funzione per generare una stanza casuale
static struct Stanza* genera_random() {

}

// Funzione per liberare tutta la mappa
static void chiudi_mappa() {

}


/**
 * Funzione per impostare il gioco
 */
void imposta_gioco() {
    struct Giocatore* giocatori[3] = {NULL, NULL, NULL};
    int num_giocatori;
    int isPrincipe = -1;  

    num_giocatori = leggi_numero("Inserisci il numero di giocatori (1-3): ");  //Usa la funzione per leggere in modo sicuro

    //Controllo sul numero di giocatori
    while (num_giocatori < 1 || num_giocatori > 3) {
        num_giocatori = leggi_numero("Numero non valido! Inserisci un numero di giocatori tra 1 e 3: ");  //Usa la funzione per leggere in modo sicuro
    }

    printf("Hai scelto %d giocatori.\n", num_giocatori);

    //Creazione dinamica dei giocatori
    for (int i = 0; i < num_giocatori; i++) {
        giocatori[i] = (struct Giocatore*) malloc(sizeof(struct Giocatore));

        if (giocatori[i] == NULL) {
            printf("Errore nell'allocazione della memoria per il giocatore %d.\n", i + 1);
            return;
        }

        inizializza_giocatore(giocatori[i], &isPrincipe, i); //inizializzazione del giocatore
    }

    //Se non e' stato scelto un principe, lo impostiamo per l'ultimo giocatore
    if (isPrincipe == -1) {
        printf("Nessun principe scelto. Il giocatore %d diventa automaticamente principe.\n", num_giocatori);
        giocatori[num_giocatori - 1]->classe_giocatore = 0;  //Impostiamo automaticamente principe
        inizializza_giocatore(giocatori[num_giocatori - 1], &isPrincipe, num_giocatori - 1);
    }

    printf("Il gioco e' pronto a partire con %d giocatori.\n", num_giocatori);
    menu_stanze();


    struct Stanza* mappa = NULL;
}


/**
 * Funzione per iniziare il gioco
 */
void gioca() {
    printf("Inizio del gioco...\n");
}

/**
 * Funzione per terminare il gioco
 */
void termina_gioco() {
    printf("Gioco terminato. Grazie per aver giocato!\n");
}

/**
 * Funzione per la visualizzazione dei crediti
 */
void crediti() {
    printf("Crediti:\n");
    printf("Sviluppato da: [Nome Sviluppatore]\n");
}

/**
 * Funzione che stampa il menu di gioco
 */
void stampaMenu() {
    printf("====== MENU DI GIOCO ======\n");
    printf("1) Imposta gioco\n");
    printf("2) Gioca\n");
    printf("3) Termina gioco\n");
    printf("4) Visualizza i crediti\n");
    printf("===========================\n");
    printf("Scegli un'opzione: ");
}
