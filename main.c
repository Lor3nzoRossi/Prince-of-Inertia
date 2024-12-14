#include <stdio.h>
#include "gamelib.h"

int main() {
    int scelta;

    stampaMenu();
    
    do {
        scanf("%d", &scelta);

        switch (scelta) {
            case 1:
                imposta_gioco();
                break;
            case 2:
                gioca();
                break;
            case 3:
                termina_gioco();
                break;
            case 4:
                crediti();
                break;
            default:
                printf("Opzione non valida! Riprova: ");
                continue;
        }

    } while (scelta != 3); //se l'utente sceglie 3, uscire dal gioco

    printf("Gioco terminato. Arrivederci!\n");
    return 0;
}
