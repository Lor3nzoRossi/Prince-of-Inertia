/*Questo file contiene le definizioni delle funzioni che implementano il gioco.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"


static void ins_stanza(void);
static struct Stanza* genera_random(int nStanze);

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

bool sceltaSiNo(char scelta){
    if(scelta == 's' || scelta == 'S'){
        return true;
    }else if(scelta == 'n' || scelta == 'N'){
        return false;
    }
    printf("Scelta non valida, inserire 's' per 'sì' e 'n' per 'no'.");  
}

/**
 * Stampa il menù delle stanze
 * permettendo la scelta di una delle opzioni
 */
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

        scelta = leggi_numero("");

        // Gestione delle opzioni
        switch (scelta) {
            case 1:
                ins_stanza();
                break;
            case 2:
                // canc_stanza();
                break;
            case 3:
                stampa_stanze();
                break;
            case 4:
                char sceltaDisclamer = 'n';
                if(pFirst){
                    printf("*ATTENZIONE* se si prosegue le precedenti stanze verranno eliminate, proseguire? (s/n)");
                    scanf(" %c", &sceltaDisclamer);
                    if(sceltaSiNo(sceltaDisclamer)){
                        genera_random(15);
                        printf("Stanze generate");
                    }else{
                        break;
                    }
                }else{
                    genera_random(15);
                }
                break;
            case 5:
                chiudi_mappa();
                printf("\nMappa chiusa. Programma terminato.\n");
                break;
            default:
                printf("\nScelta non valida, scegliere tra 1-5\n");
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
 * Permette di selezionare un tipo di stanza
 * @returns enum Tipo_stanza selezionato
 */
enum Tipo_stanza scegli_tipoStanza() {
    int scelta;
    do{
        printf("Scegli il tipo di stanza:\n");
        printf("0 - Corridoio\n");
        printf("1 - Scala\n");
        printf("2 - Sala banchetto\n");
        printf("3 - Magazzino\n");
        printf("4 - Posto guardia\n");
        printf("5 - Prigione\n");
        printf("6 - Armeria\n");
        printf("7 - Moschea\n");
        printf("8 - Torre\n");
        printf("9 - Bagni\n");
        scelta = leggi_numero("");
        printf("\n");
        if(scelta < 0 || scelta > 9){
            printf("Input non valido! Scegliere tra 0-9\n");
        }
    }while(scelta < 0 || scelta > 9);
    return (enum Tipo_stanza)scelta;
}

/**
 * Permette di selezionare un tipo di trabocchetto
 * @returns enum Tipo_trabocchetto selezionato
 */
enum Tipo_trabocchetto scegli_trabocchetto() {
    int scelta;
    do
    {
        printf("Scegli il tipo di trabocchetto:\n");
        printf("0 - Nessuno\n");
        printf("1 - Tegola\n");
        printf("2 - Lame\n");
        printf("3 - Caduta\n");
        printf("4 - Burrone\n");
        scelta = leggi_numero("");
        if(scelta < 0 || scelta > 4){
            printf("Input non valido! Scegliere tra 0-4\n");
        }
    } while (scelta < 0 || scelta > 4);
    
    return (enum Tipo_trabocchetto)scelta;
}

/**
 * Permette di selezionare un tipo di tesoro
 * @returns enum Tipo_tesoro selezionato
 */
enum Tipo_tesoro scegli_tesoro() {
    int scelta;
    do
    {
        printf("Scegli il tipo di tesoro:\n");
        printf("0 - Nessun tesoro\n");
        printf("1 - Verde veleno\n");
        printf("2 - Blu guarigione\n");
        printf("3 - Rosso aumenta vita\n");
        printf("4 - Spada tagliente\n");
        printf("5 - Scudo\n");
        scelta = leggi_numero("");
        if(scelta < 0 || scelta > 5){
            printf("Input non valido! Scegliere tra 0-5\n");
        }
    } while (scelta < 0 || scelta > 5);
    
    return (enum Tipo_tesoro)scelta;
}




/**
 * Permette di inserire una nuova stanza
 * scegliendone la posizione rispetto all'ultima creata
 */
static void ins_stanza() {
    struct Stanza* nuovaStanza = (struct Stanza*)malloc(sizeof(struct Stanza)); //creazione spazio di memoria per nuova stanza
    //controllo se spazio in memoria è sufficiente per la creazione della stanza
    if (!nuovaStanza) {
        printf("Errore: memoria insufficiente.\n");
        return;
    }

    //inserimento parametri della nuova stanza
    nuovaStanza->tipo_stanza = scegli_tipoStanza();

    nuovaStanza->trabocchetto = scegli_trabocchetto();
    nuovaStanza->tesoro = scegli_tesoro();

    //nel caso non fosse stata ancora aggiunta nessuna stanza
    if (pFirst == NULL) {
        pFirst = nuovaStanza;
        pUltima = nuovaStanza;
        printf("Prima stanza aggiunta con successo!\n");
        return;
    }

    //nel caso esistesse già una stanza nella lista, chiedere dove creare questa nuova stanza
    char direzione[10];
    bool creata = false;
    do
    {
        printf("In quale direzione, rispetto all'ultima stnaza, vuoi aggiungere la nuova stanza? (sopra/sotto/sinistra/destra): ");
        scanf("%s", direzione);

        //inserimento della nuova stanza nella direzione scelta
        if (strcmp(direzione, "sopra") == 0) {
            creata = true;
            if (pUltima->stanza_sopra != NULL) {
                printf("Errore: esiste gia' una stanza sopra.\n");
            } else {
                pUltima->stanza_sopra = nuovaStanza;
                printf("Stanza aggiunta sopra con successo!\n");
            }
        } else if (strcmp(direzione, "sotto") == 0) {
            creata = true;
            if (pUltima->stanza_sotto != NULL) {
                printf("Errore: esiste gia' una stanza sotto.\n");
            } else {
                pUltima->stanza_sotto = nuovaStanza;
                printf("Stanza aggiunta sotto con successo!\n");
            }
        } else if (strcmp(direzione, "sinistra") == 0) {
            creata = true;
            if (pUltima->stanza_sinistra != NULL) {
                printf("Errore: esiste gia' una stanza a sinistra.\n");
            } else {
                pUltima->stanza_sinistra = nuovaStanza;
                printf("Stanza aggiunta a sinistra con successo!\n");
            }    
        } else if (strcmp(direzione, "destra") == 0) {
            creata = true;
            if (pUltima->stanza_destra != NULL) {
                printf("Errore: esiste gia' una stanza a destra.\n");
            } else {
                pUltima->stanza_destra = nuovaStanza;
                printf("Stanza aggiunta a destra con successo!\n");
            }
        } else {
            printf("Direzione non valida.\n");
        }
    } while (!creata);
    
    
    pUltima = nuovaStanza; //impostare la stanza appena creata come l'ultima
}

/**
 * da fare
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
 * Recupera il nome del tipo della stanza dato un numero da 0 - 9 (in base alle scelte nel menù)
 * @returns stringa del nome della stanza
 */
const char* get_tipoStanza(int nStanza) {
    switch(nStanza) {
        case 0:
            return "Corridoio";
        case 1:
            return "Scala";
        case 2:
            return "Sala banchetto";
        case 3:
            return "Magazzino";
        case 4:
            return "Posto guardia";
        case 5:
            return "Prigione";
        case 6:
            return "Armeria";
        case 7:
            return "Moschea";
        case 8:
            return "Torre";
        case 9:
            return "Bagni";
        default:
            return "Tipo di stanza sconosciuto";
    }
}

/**
 * Recupera il nome del trabocchetto dato un numero da 0 - 9 (in base alle scelte nel menù)
 * @returns stringa del nome del trabocchetto
 */
const char* get_trabocchetto(int nStanza) {
    switch(nStanza) {
        case 0:
            return "Nessuno";
        case 1:
            return "Tegola";
        case 2:
            return "Lame";
        case 3:
            return "Caduta";
        case 4:
            return "Burrone";
        default:
            return "Tipo di trabocchetto sconosciuto";
    }
}

/**
 * Recupera il nome del tesoro dato un numero da 0 - 9 (in base alle scelte nel menù)
 * @returns stringa del nome del tesoro
 */
const char* get_tesoro(int nStanza) {
    switch(nStanza) {
        case 0:
            return "Nessun tesoro";
        case 1:
            return "Verde veleno";
        case 2:
            return "Blu guarigione";
        case 3:
            return "Rosso aumenta vita";
        case 4:
            return "Spada tagliente";
        case 5:
            return "Scudo";
        default:
            return "Tipo di tesoro sconosciuto";
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
    printf("Elenco delle stanze:\n\n");
    while (corrente != NULL) {
        if(index == 1){
            printf("Prima stanza:\n");
        }
        printf("Tipo di stanza: %s\n", get_tipoStanza(corrente->tipo_stanza));
        printf("Trabocchetto: %s\n", get_trabocchetto(corrente->trabocchetto));
        printf("Tesoro: %s\n\n", get_tesoro(corrente->tesoro));
        index += 1;
        //stampa stanza sopra, se presente
        if (corrente->stanza_sopra) {
            printf("Stanza sopra:\n");
            corrente = corrente->stanza_sopra;
        }
        //stampa stanza a destra, se presente
        else if (corrente->stanza_destra) {
            printf("Stanza a destra:\n");
            corrente = corrente->stanza_destra;
        }
        //stampa stanza sotto, se presente
        else if (corrente->stanza_sotto) {
            printf("Stanza sotto:\n");
            corrente = corrente->stanza_sotto;
        }
        //stampa stanza a sinistra, se presente
        else if (corrente->stanza_sinistra) {
            printf("Stanza a sinistra:\n");
            corrente = corrente->stanza_sinistra;
        } else {
            break; //se non ci sono stanze adiacenti, termina il ciclo
        }
    }

}

/**
 * Elimina la mappa di gioco, se presente almeno una stanza
 */
bool elimina_mappa(){
    bool eliminata = false;
    if(pFirst){
        pFirst->stanza_sopra = NULL;
        pFirst->stanza_sotto = NULL;
        pFirst->stanza_destra = NULL;
        pFirst->stanza_sinistra = NULL;
        pFirst = NULL;
        eliminata = true;
    }else{
        return eliminata;
    }
}

/**
 * Cancella tutte le stanze e crea un numero variabile di nuove stanze
 * @param nStanze numero di stanze da generare
 */
static struct Stanza* genera_random(int nStanze) {
    srand(time(NULL)); //inizializza il generatore di numeri casuali
    bool primo = true;

    for (int i = 0; i < nStanze; i++) {
        struct Stanza* nuova_stanza = (struct Stanza*)malloc(sizeof(struct Stanza)); //allocazione memoria per la stanza

        //controllo se spazio disponibile per nuova stanza esiste
        if (nuova_stanza == NULL) {
            printf("Errore nell'allocazione della memoria per la stanza!\n");
            return NULL; 
        }

        //assegnazione del tipo di trabocchetto
        enum Tipo_trabocchetto trabocchetto;
        int n_random_trabocchetto = rand() % 100;

        if (n_random_trabocchetto < 65) { //65%
            trabocchetto = nessuno;
        } else if (n_random_trabocchetto < 75) { //10%
            trabocchetto = tegola;
        } else if (n_random_trabocchetto < 84) {//9%
            trabocchetto = lame;
        } else if (n_random_trabocchetto < 92) {//8%
            trabocchetto = caduta;
        } else { //8%
            trabocchetto = burrone;
        }

        //assegnazione del tipo di tesoro
        enum Tipo_tesoro tesoro;
        int n_random_tesoro = rand() % 100;
        if (n_random_tesoro < 20) { //20%
            tesoro = nessun_tesoro;
        } else if (n_random_tesoro < 40) { //20%
            tesoro = verde_veleno;
        } else if (n_random_tesoro < 60) { //20%
            tesoro = blu_guarigione;
        } else if (n_random_tesoro < 75) { //15%
            tesoro = rosso_aumenta_vita;
        } else if (n_random_tesoro < 90) { //15%
            tesoro = burrone;
        } else { //10%
            tesoro = scudo;
        }

        enum Tipo_stanza tipo_stanza = (enum Tipo_stanza)(rand() % 10); //assegnazione tipo stanza

        //assegnazione dati a nuova stanza
        nuova_stanza->stanza_destra = NULL;
        nuova_stanza->stanza_sinistra = NULL;
        nuova_stanza->stanza_sopra = NULL;
        nuova_stanza->stanza_sotto = NULL;
        nuova_stanza->tipo_stanza = tipo_stanza;
        nuova_stanza->tesoro = tesoro;
        nuova_stanza->trabocchetto = trabocchetto;

        //imposta la nuova stanza come prima stanza, se è la prima
        if (primo) {
            pFirst = nuova_stanza;
            primo = false;
        } else if (i == nStanze - 1) {
            //imposta la nuova stanza come ultima stanza, se è l'ultima
            pUltima = nuova_stanza;
        }
    }
    return pFirst; //ritorna il puntatore alla prima stanza
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

    char scelta = 's';
    bool primo = true;
    printf("Ora il game master creera' la mappa di gioco:\n");
    do
    {
        if(primo){
            printf("Iniziare la creazione della mappa dalla generazione di 15 stanze con dati casuali? (s/n)\n");
            scanf(" %c", &scelta);  
            if(sceltaSiNo(scelta)){
                printf("Generazione di 15 stanze casuali...\n");
                genera_random(15);  
                printf("Generazione completata.\n");

                printf("Si desidera apportare delle modifiche alla mappa di gioco gia' creata? (s/n)\n");
                scanf(" %c", &scelta); 
                if (sceltaSiNo(scelta)) {
                    menu_stanze();  
                }else{
                    gioca();
                    break;
                }
            }else{
                menu_stanze();
            }
            primo = false;
        }else{
            menu_stanze();
        }
    } while (scelta == 's' || scelta == 'S' || scelta == 'n' || scelta == 'N');
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
