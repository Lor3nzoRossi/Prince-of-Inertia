/*Questi file contiene le dichiarazioni delle funzioni definite in "gamelib.c" (solo quelle non static) e
le definizioni del tipo delle strutture dati utilizzate in "gamelib.c"*/

#define GAMELIB_H  
#define MAX_PLAYERS 3
#include <stdbool.h>


//ENUM

enum tipo_giocatore {
    principe,
    doppleganger
};

enum Tipo_trabocchetto {
    nessuno,
    tegola,
    lame,
    caduta,
    burrone
};

enum Tipo_tesoro {
    nessun_tesoro,
    verde_veleno,
    blu_guarigione,
    rosso_aumenta_vita,
    spada_tagliente,
    scudo
};

enum Tipo_stanza {
    corridoio,
    scala,
    sala_banchetto,
    magazzino,
    posto_guardia,
    prigione,
    armeria,
    moschea,
    torre,
    bagni
};


//STRUCT

struct Stanza {
    struct Stanza* stanza_destra;
    struct Stanza* stanza_sinistra;
    struct Stanza* stanza_sopra;
    struct Stanza* stanza_sotto;
    enum Tipo_stanza tipo_stanza;
    enum Tipo_trabocchetto trabocchetto;
    enum Tipo_tesoro tesoro;
};

struct Stanza* pFirst;
struct Stanza* pUltima;


struct Giocatore {
    char nome_giocatore[100];
    enum tipo_giocatore classe_giocatore;
    struct Stanza* posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
};

static struct Giocatore* giocatori[MAX_PLAYERS];

//FUNZIONI

void stampaMenu();  


void imposta_gioco();
int leggi_numero(const char* messaggio);
void inizializza_giocatore(struct Giocatore* giocatore, int numGiocatore);

enum Tipo_stanza scegli_tipoStanza();
enum Tipo_trabocchetto scegli_trabocchetto();
enum Tipo_tesoro scegli_tesoro();

const char* get_tipoStanza(int nStanza);
const char* get_trabocchetto(int nStanza);
const char* get_tesoro(int nStanza);

void stampa_stanze();

static bool chiudi_mappa();
static bool elimina_mappa();

void gioca();  
void termina_gioco();  
void crediti();  
