/*Questo file contiene le definizioni delle funzioni che implementano il gioco.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
 * @param numGiocatore numero della creazione del giocatore
 */
void inizializza_giocatore(struct Giocatore* giocatore, int numGiocatore) {
    numGiocatore += 1; //aumento di 1 per visualizzazione

    //INSERIMENTO DATI GIOCATORE

    printf("Inserire il nome per il giocatore: ");
    scanf("%s", &giocatore->nome_giocatore); 

    giocatore->p_vita_max = leggi_numero("Inserire i punti vita massimi per il giocatore: ");
    giocatore->p_vita = giocatore->p_vita_max;  //imposta i punti vita correnti uguali a quelli massimi

    giocatore->dadi_attacco = leggi_numero("Inserire il numero di dadi attacco per il giocatore: ");

    giocatore->dadi_difesa = leggi_numero("Inserire il numero di dadi difesa per il giocatore: ");

    giocatore->posizione = NULL; //inizializzazione della posizione (di default NULL)
}


/**
 * La funzione ins_stanza() inserisce una nuova stanza in memoria, 
 * collegandola alla lista esistente secondo una direzione scelta.
 */
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

/**
 * stampa tutte le stanze presenti
 */
void stampa_stanze() {
    //controlla se ci sono stanze
    if (pFirst == NULL) {
        printf("La lista delle stanze è vuota.\n");
        return;
    }

    struct Stanza* corrente = pFirst; //puntatore per scorrere la lista
    int index = 1; //contatore per numerare le stanze
    printf("Elenco delle stanze:\n");
    while (corrente != NULL) {
        //scorrere tutte le stanze somehow
    }
}

/**
 * Cancella tutte le stanze e ne crea altre 15 con valori casuali
 * 
 */
static struct Stanza* genera_random() {

}

// Funzione per liberare tutta la mappa
static void chiudi_mappa() {

}


/**
 * Funzione per impostare il gioco
 */
void imposta_gioco() {
    int num_giocatori;

    num_giocatori = leggi_numero("Inserisci il numero di giocatori (1-3): ");

    //controllo sul numero di giocatori
    while (num_giocatori < 1 || num_giocatori > 3) {
        num_giocatori = leggi_numero("Numero non valido! Inserisci un numero di giocatori tra 1 e 3: "); 
    }

    printf("Hai scelto %d giocatori.\n", num_giocatori);

    if(num_giocatori > 1){
        //creazione dinamica dei giocatori
        for (int i = 0; i < num_giocatori; i++) {
            giocatori[i] = (struct Giocatore*) malloc(sizeof(struct Giocatore)); //creazione area di memoria

            if (giocatori[i] == NULL) {
                printf("Errore nell'allocazione della memoria per il giocatore %d.\n", i + 1);
                return;
            }

            bool trovatoPrincipe = false;
            for (int j = 0; j < num_giocatori; j++) {
                if (giocatori[j] != NULL && giocatori[j]->classe_giocatore == 0) {
                    trovatoPrincipe = true;
                    break; 
                }
            }


            if (trovatoPrincipe) {
                printf("Dato che e' gia' stato scelto un principe, verra' automaticamente assegnata la classe doppleganger\n");
                giocatori[i]->classe_giocatore = 1;
            } else {
                if (i == num_giocatori - 1) {
                    printf("Non e' stato scelto alcun principe, assegnazione automatica a principe...\n");
                    giocatori[i]->classe_giocatore = 0;
                } else {
                    giocatori[i]->classe_giocatore = leggi_numero("Inserisci 0 per principe o 1 per doppleganger ");
                }
            }       

            inizializza_giocatore(giocatori[i], i); //inizializzazione del giocatore con i dati comuni ad entrambe le classi
        }
    }else{
        giocatori[0] = (struct Giocatore*) malloc(sizeof(struct Giocatore));

        if (giocatori[0] == NULL) {
            printf("Errore nell'allocazione della memoria per il giocatore");
            return;
        }

        printf("Per un solo giocatore, verra' attribuita l'unica classe disponibile è il principe\n");
        giocatori[0]->classe_giocatore = 0; //impostazione classe a principe

        inizializza_giocatore(giocatori[0], 0);
    }

    printf("GIOCATORI:\n");
    for(int i = 0; i<num_giocatori; i++){
        printf("- %s (classe %d)\n", giocatori[i]->nome_giocatore, giocatori[i]->classe_giocatore);
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
