/*Questo file contiene le definizioni delle funzioni che implementano il gioco.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"

static int mappa_chiusa = 0; // 0 = mappa aperta, 1 = mappa chiusa
static int num_stanze = 0; //contatore stanze
static int num_giocatori = 0; //contatore numero di giocatori

static bool chiudi_mappa();
static void ins_stanza(void);
static bool sceltaSiNo(char scelta);
static void stampa_stanza(struct Stanza* stanza);
static struct Stanza* genera_random();

static void attiva_trabocchetto(struct Stanza* stanza);
static void avanza(struct Giocatore* giocatore);
static void passa(int* turno);
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

/**
 * Analizza la scelta 
 * @returns true se la scelta è 's' o 'S'
 * @returns false se la scelta è 'n' o 'N'
 */
static bool sceltaSiNo(char scelta){
    if(scelta == 's' || scelta == 'S'){
        return true;
    }else if(scelta == 'n' || scelta == 'N'){
        return false;
    }
    printf("Scelta non valida, inserire 's' per 'si' e 'n' per 'no'.");  
}

/**
 * Stampa il menù delle stanze
 * permettendo la scelta di una delle opzioni
 */
bool menu_stanze() {
    int scelta;
    bool esci = false;
    printf("Entrato in menu_stanze.\n"); // Debug
    do {
        printf("\n==== MENU GESTIONE STANZE ====");
        printf("\n1. Inserisci una nuova stanza");
        printf("\n2. Cancella una stanza");
        printf("\n3. Stampa tutte le stanze");
        printf("\n4. Genera mappa causale");
        printf("\n5. Chiudi la mappa e termina");
        printf("\nSeleziona un'opzione (1-5): ");

        scelta = leggi_numero("");

        switch (scelta) {
            case 1:
                ins_stanza();
                break;
            case 2:
                // canc_stanza();
                break;
            case 3:
                // stampa_stanze();
                break;
            case 4:
                char sceltaDisclamer = 'n';
                if (pFirst) {
                    printf("*ATTENZIONE* se si prosegue le precedenti stanze verranno eliminate, proseguire? (s/n)");
                    scanf(" %c", &sceltaDisclamer);
                    if (sceltaSiNo(sceltaDisclamer)) {
                        genera_random();
                    } else {
                        break;
                    }
                } else {
                    genera_random();
                }
                break;
            case 5:
                if (chiudi_mappa()) {
                    esci = true;
                } else {
                    printf("Impossibile terminare la creazione della mappa, numero di stanze insufficenti: %d. Sono necessarie almeno 15 stanze\n", num_stanze);
                }
                break;
            default:
                printf("\nScelta non valida, scegliere tra 1-5\n");
        }
    } while (!esci);
    printf("Terminata la creazione della mappa.\n");
    gioca();
    return false;
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

    num_giocatori++;
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
        printf("In quale direzione, rispetto all'ultima stanza, vuoi aggiungere la nuova stanza? (sopra/sotto/sinistra/destra): ");
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
    num_stanze++;
}

/**
 * Elimina l'ultima stanza
 */
static void canc_stanza() {
    if (pUltima) {
        struct Stanza* stanza_appoggio = pUltima;

        if (pUltima->stanza_destra) {
            pUltima = stanza_appoggio->stanza_destra;
            free(stanza_appoggio->stanza_destra);
            stanza_appoggio->stanza_destra = NULL;
        } else if (pUltima->stanza_sinistra) {
            pUltima = stanza_appoggio->stanza_sinistra;
            free(stanza_appoggio->stanza_sinistra);
            stanza_appoggio->stanza_sinistra = NULL;
        } else if (pUltima->stanza_sopra) {
            pUltima = stanza_appoggio->stanza_sopra;
            free(stanza_appoggio->stanza_sopra);
            stanza_appoggio->stanza_sopra = NULL;
        } else if (pUltima->stanza_sotto) {
            pUltima = stanza_appoggio->stanza_sotto;
            free(stanza_appoggio->stanza_sotto);
            stanza_appoggio->stanza_sotto = NULL;
        } else {
            free(pUltima);
            pUltima = NULL;
        }
        printf("L'ultima stanza e' stata cancellata.\n");
    } else {
        printf("Non ci sono stanze da cancellare.\n");
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
 * stampa tutte le stanze presenti (da fare)
 */
void stampa_stanze() {
    if (pFirst == NULL) {
        printf("La lista delle stanze e' vuota.\n");
        return;
    }
}

static void stampa_stanza(struct Stanza* stanza){
    printf("Tipo stanza: %s\n", get_tipoStanza(stanza->tipo_stanza));
    printf("Trabocchetto: %s\n", get_trabocchetto(stanza->trabocchetto));
    printf("Tesoro: %s\n", get_tesoro(stanza->tesoro));
}





// /**
//  * Elimina la mappa di gioco, se presente almeno una stanza
//  * @returns true se la mappa è stata eliminata, false se non ci sono stanze da eliminare
//  */
// static bool elimina_mappa() {
//     struct Stanza* corrente = pFirst;
//     bool eliminata = false;

//     while (corrente != NULL) {
//         struct Stanza* stanza_successiva = NULL;

//         if (corrente->stanza_destra) {
//             stanza_successiva = corrente->stanza_destra;
//         } else if (corrente->stanza_sinistra) {
//             stanza_successiva = corrente->stanza_sinistra;
//         } else if (corrente->stanza_sopra) {
//             stanza_successiva = corrente->stanza_sopra;
//         } else if (corrente->stanza_sotto) {
//             stanza_successiva = corrente->stanza_sotto;
//         }

//         canc_stanza();

//         corrente = stanza_successiva;
        
//         eliminata = true;
//         num_stanze--;
//     }

//     return eliminata;
// }


/**
 * Cancella tutte le stanze e crea un numero variabile di nuove stanze
 * @param nStanze numero di stanze da generare
 */
static struct Stanza* genera_random() {
    srand(time(NULL)); 
    bool primo = true;
    
    struct Stanza* stanze[MIN_STANZE]; 
    
    for (int i = 0; i < MIN_STANZE; i++) {
        struct Stanza* nuova_stanza = (struct Stanza*)malloc(sizeof(struct Stanza)); //allocazione nuova stanza in memoria

        //controllo sull'allocamento della nuova stanza in memoria
        if (nuova_stanza == NULL) {
            printf("Errore nell'allocazione della memoria per la stanza!\n");
            return NULL; 
        }

        //assegnazione tipo trabocchetto
        enum Tipo_trabocchetto trabocchetto;
        int n_random_trabocchetto = rand() % 100;

        if (n_random_trabocchetto < 65) {
            trabocchetto = nessuno;
        } else if (n_random_trabocchetto < 75) {
            trabocchetto = tegola;
        } else if (n_random_trabocchetto < 84) {
            trabocchetto = lame;
        } else if (n_random_trabocchetto < 92) {
            trabocchetto = caduta;
        } else {
            trabocchetto = burrone;
        }

        //assegnazione tipo tesoro
        enum Tipo_tesoro tesoro;
        int n_random_tesoro = rand() % 100;
        if (n_random_tesoro < 20) {
            tesoro = nessun_tesoro;
        } else if (n_random_tesoro < 40) {
            tesoro = verde_veleno;
        } else if (n_random_tesoro < 60) {
            tesoro = blu_guarigione;
        } else if (n_random_tesoro < 75) {
            tesoro = rosso_aumenta_vita;
        } else if (n_random_tesoro < 90) {
            tesoro = burrone;
        } else {
            tesoro = scudo;
        }

        //assegnazione tipo stanza 
        enum Tipo_stanza tipo_stanza = (enum Tipo_stanza)(rand() % 10);

        nuova_stanza->stanza_destra = NULL;
        nuova_stanza->stanza_sinistra = NULL;
        nuova_stanza->stanza_sopra = NULL;
        nuova_stanza->stanza_sotto = NULL;
        nuova_stanza->tipo_stanza = tipo_stanza;
        nuova_stanza->tesoro = tesoro;
        nuova_stanza->trabocchetto = trabocchetto;

        stanze[i] = nuova_stanza;

        //collegamento stanza ad una casuale
        if (i > 0) {
            int direzione = rand() % 4;
            switch (direzione) {
                case 0: //sopra
                    stanze[i-1]->stanza_sotto = nuova_stanza;
                    nuova_stanza->stanza_sopra = stanze[i-1];
                    break;
                case 1: //sotto
                    stanze[i-1]->stanza_sopra = nuova_stanza;
                    nuova_stanza->stanza_sotto = stanze[i-1];
                    break;
                case 2: //destra
                    stanze[i-1]->stanza_sinistra = nuova_stanza;
                    nuova_stanza->stanza_destra = stanze[i-1];
                    break;
                case 3: //sinistra
                    stanze[i-1]->stanza_destra = nuova_stanza;
                    nuova_stanza->stanza_sinistra = stanze[i-1];
                    break;
            }
        }

        //prima stanza
        if (primo) {
            pFirst = nuova_stanza;
            primo = false;
        } else if (i == MIN_STANZE - 1) { //ultima stanza
            pUltima = nuova_stanza;
        }

        num_stanze++;
    }

    return pFirst;
}


/**
 * Termina la creazione della mappa
 * @returns true se la creazione della mappa può essere terminata
 * @returns se è impossibile terminarla
 */
static bool chiudi_mappa() {
    if(num_stanze >= MIN_STANZE){
        mappa_chiusa = 1;
        return true;
    }else{
        mappa_chiusa = 0;
        return false;
    }
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

        printf("Per un solo giocatore, verra' attribuita l'unica classe disponibile e' il principe\n");
        giocatori[0]->classe_giocatore = 0; //impostazione classe a principe

        inizializza_giocatore(giocatori[0], 0);
    }

    printf("GIOCATORI:\n");
    for(int i = 0; i<num_giocatori; i++){
        printf("- %s (classe %d)\n", giocatori[i]->nome_giocatore, giocatori[i]->classe_giocatore);
    }

    char scelta = 's';
    bool primo = true;
    bool esci = false;
    printf("Ora il game master creera' la mappa di gioco:\n");
    do {
        if (primo) {
            primo = false;
            printf("Iniziare la creazione della mappa dalla generazione di 15 stanze con dati casuali? (s/n)\n");
            scanf(" %c", &scelta);  
            if (sceltaSiNo(scelta)) {
                printf("Generazione di 15 stanze casuali...\n");
                genera_random();  
                printf("Generazione completata.\n");
                mappa_chiusa = 1;
                printf("Si desidera apportare delle modifiche alla mappa di gioco gia' creata? (s/n)\n");
                scanf(" %c", &scelta); 
                if (sceltaSiNo(scelta)) {
                    if(!menu_stanze()){
                        esci = true;
                    } 
                } else {
                    gioca();
                    esci = true;
                }
            } else {
                if(!menu_stanze()){
                    esci = true;
                }  
            }
        } else {
            if(!menu_stanze()){
                esci = true;
            }
        }
    } while (!esci);  // Ensure the loop does not re-enter


}

/**
 * Determina chi inizia ad attaccare e difendere per primo nel combattimento
 * @param giocatore giocatore che deve combattere
 * @param tipo_nemico stringa che contiene il tipo di nemico
 * @returns true se inizia ad attaccare il giocatore
 * @returns false se inizia ad attaccare il nemico
 */
static bool inizio_combattimento(struct Giocatore* giocatore, char* tipo_nemico) {
    int dado_giocatore = 0;
    int dado_nemico = 0;

    do {
        printf("Il giocatore lancia il dado...\n", giocatore->nome_giocatore);
        dado_giocatore = rand() % 6 + 1; // lancio dado giocatore
        printf("Il giocatore totalizza %d\n", dado_giocatore);

        printf("%s lancia il dado...\n", tipo_nemico);
        dado_nemico = rand() % 6 + 1; // lancio dado nemico
        printf("%s totalizza %d\n", tipo_nemico, dado_nemico);

        if (dado_giocatore > dado_nemico) {
            return true; // true se vince il giocatore
        } else if (dado_giocatore < dado_nemico) {
            return false; // false se vince il nemico
        } else {
            printf("Il risultato è un pareggio, la procedura verrà ripetuta.\n");
        }
    } while (dado_giocatore == dado_nemico);

    return false;
}


/**
 * Performa il combattimento contro uno scheletro
 * @param giocatore giocatore che deve combattere
 */
static void combatti_scheletro(struct Giocatore* giocatore){
    printf("Lancio del dado per determinare chi inizia ad attaccare...\n");
    if(inizio_combattimento(giocatore, "scheletro")){
        printf("Il giocatore vince il sorteggio!\n");
    }else{
        printf("Lo scheletro vince il sorteggio!\n");
    }
}

/**
 * Performa il combattimento contro una guardia
 * @param giocatore giocatore che deve combattere
 */
static void combatti_guardia(struct Giocatore* giocatore){
    printf("Lancio del dado per determinare chi inizia ad attaccare...\n");
    if(inizio_combattimento(giocatore, "guardia")){
        printf("Il giocatore vince il sorteggio!\n");
    }else{
        printf("Lo guardia vince il sorteggio!\n");
    }
}

/**
 * Performa il combattimento contro Jaffar
 * @param giocatore giocatore che deve combattere
 */
static void combatti_Jaffar(struct Giocatore* giocatore){
    printf("Lancio del dado per determinare chi inizia ad attaccare...\n");
    if(inizio_combattimento(giocatore, "Jaffar")){
        printf("Il giocatore vince il sorteggio!\n");
    }else{
        printf("Jaffar vince il sorteggio!\n");
    }
}

/**
 * Performa il combattimento contro un nemico
 * @param giocatore giocatore che deve combattere
 * @param tipo_nemico tipo di nemico
 */
static void combatti(struct Giocatore* giocatore, char* tipo_nemico) {
    if(strcmp(tipo_nemico, "scheletro") == 0){
        combatti_scheletro(giocatore);
    }else if(strcmp(tipo_nemico, "guardia") == 0){
        combatti_guardia(giocatore);
    }else if(strcmp(tipo_nemico, "Jaffar") == 0){
        combatti_Jaffar(giocatore);
    }else{
        printf("Tipo di nemico sconosciuto.");
    }
}

/**
 * Ha la possibilità di fare apparire un nemico
 * @param ultima true se è l'ultima stanza
 * @param ultima false se non è l'ultima stanza
 */
static void enemy_spawn(struct Giocatore* giocatore, bool ultima){
    if(ultima){
        printf("\n||Appare Jaffar||\n");
        combatti(giocatore, "Jaffar");
    }else{
        int rand_num = rand() % 100;

        if(rand_num<60){ //60%
            printf("\n||Appare uno scheletro||\n\n");
            combatti(giocatore, "scheletro");
        }else { //40%
            printf("\n||Appare una guardia||\n\n");
            combatti(giocatore, "guardia");
        }
    }
}

/**
 * Attiva il trabocchetto di una stanza
 * @param stanza stanza di cui attivare il trabocchetto
 */
static void attiva_trabocchetto(struct Stanza* stanza) {
    if (stanza->trabocchetto) {
        printf("Si attiva %s nella stanza!\n", get_trabocchetto(stanza->trabocchetto));
    }
}

static void avanza(struct Giocatore* giocatore) {
    struct Stanza* stanze_adiacenti[4];
    int num_stanze_valide = 0; //conta il numero di stanze valide
    char* direzioni[4] = {"su", "giu", "destra", "sinistra"};
    int direzioni_possibili[4];  //direzioni valide

    //controlla le stanze adiacenti e aggiungi quelle valide all'array
    if (giocatore->posizione->stanza_sopra != NULL) {
        stanze_adiacenti[num_stanze_valide] = giocatore->posizione->stanza_sopra;
        direzioni_possibili[num_stanze_valide] = 0;
        num_stanze_valide++;
    }
    if (giocatore->posizione->stanza_sotto != NULL) {
        stanze_adiacenti[num_stanze_valide] = giocatore->posizione->stanza_sotto;
        direzioni_possibili[num_stanze_valide] = 1;
        num_stanze_valide++;
    }
    if (giocatore->posizione->stanza_destra != NULL) {
        stanze_adiacenti[num_stanze_valide] = giocatore->posizione->stanza_destra;
        direzioni_possibili[num_stanze_valide] = 2;
        num_stanze_valide++;
    }
    if (giocatore->posizione->stanza_sinistra != NULL) {
        stanze_adiacenti[num_stanze_valide] = giocatore->posizione->stanza_sinistra;
        direzioni_possibili[num_stanze_valide] = 3;
        num_stanze_valide++;
    }

    char scelta[10];
    bool scelta_valida = false;
    if(num_stanze_valide<=0){
        printf("non ci sono stanze valide\n");
    }
    while (!scelta_valida) {
        //direzioni possibili
        printf("(%s) In quale direzione ci si vuole muovere? ( direzioni possibili: ", giocatore->nome_giocatore);
        for (int i = 0; i < num_stanze_valide; i++) {
            printf("%s ", direzioni[direzioni_possibili[i]]);
            if (i < num_stanze_valide - 1) {
                printf(", ");
            }
        }
        printf(") ");
        scanf("%s", &scelta);

        //muovi nella direzione scelta
        for (int i = 0; i < num_stanze_valide; i++) {
            if (strcmp(scelta, direzioni[direzioni_possibili[i]]) == 0) {
                giocatore->posizione = stanze_adiacenti[i];
                printf("Ti muovi verso: %s\n", scelta);
                scelta_valida = true;
                break;
            }
        }

        if (scelta_valida) {
            if(giocatore->posizione == pUltima){
                enemy_spawn(giocatore ,true);
            }else{
                enemy_spawn(giocatore, false);
            }
        }else{
            printf("Direzione non valida, per favore scegli tra le direzioni possibili.\n");
        }
    }

    attiva_trabocchetto(giocatore->posizione);//attivazione trabocchetto
}

/**
 * Incrementa il turno di gioco
 * @param turno turno di gioco da incrementare
 */
static void passa(int* turno) {
    *turno = (*turno + 1) % num_giocatori; //incremento turno

    printf("Il turno e' passato a %s.\n", giocatori[*turno]->nome_giocatore);
}


/**
 * Stampa l'ordine dei turni di gioco
 * @param ordine_turni array contenente l'ordine dei turni 
 */
static void stampa_turni(int ordine_turni[]) {
    printf("Ordine di gioco:\n");
    for (int i = 0; i < num_giocatori; i++) {
        printf("%d) %s\n", i + 1, giocatori[ordine_turni[i]]->nome_giocatore); // corrected index for 1-based counting
    }
}

/**
 * Randomizza i turni di gioco dei giocatori
 * @param ordine_turni array che tiene traccia dell'ordine dei turni
 */
static void randomize_turns(int ordine_turni[MAX_PLAYERS]) {
    printf("Decisione del nuovo ordine di gioco...\n");
    for (int i = 0; i < num_giocatori; i++) {
        int j = rand() % num_giocatori;
        int temp = ordine_turni[i];
        ordine_turni[i] = ordine_turni[j];
        ordine_turni[j] = temp;
    }
    stampa_turni(ordine_turni);
}


/**
 * Funzione per iniziare il gioco
 */
void gioca() {
    if(mappa_chiusa == 1){
        printf("Inizio del gioco...\n");
        
        //inizializzazione posizione giocatori alla prima stanza
        for(int i = 0; i < num_giocatori; i++){
            giocatori[i]->posizione = pFirst;
        }
        
        //array per tracciare l'ordine casuale dei turni
        int ordine_turni[MAX_PLAYERS];

        for(int i = 0; i < num_giocatori; i++) {
            ordine_turni[i] = i; 
        }

        randomize_turns(ordine_turni);

        int turno = 0;
        while(true) {
            printf("E' il turno di %s\n", giocatori[ordine_turni[turno]]->nome_giocatore);
            
            avanza(giocatori[ordine_turni[turno]]);
            
            char scelta;
            printf("Vuoi passare il turno? (s/n): ");
            scanf(" %c", &scelta);
            if(sceltaSiNo(scelta)) {
                if(turno >= num_giocatori - 1){
                    printf("Rotazione conclusa.\n");
                    randomize_turns(ordine_turni); //nuova randomizzazione dei turni a fine rotazione
                    turno = 0;
                }else{
                    passa(&turno);
                }
            }
            
            

            








            // // Verifica se il gioco è terminato
            // if(condizione_fine_gioco()) {
            //     printf("Il gioco è finito.\n");
            //     break;
            // }
        }
    } else {
        printf("Impossibile iniziare il gioco.\n");
    }
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
