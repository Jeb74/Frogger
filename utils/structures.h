#ifndef FROGGER_STRUCTURES_H
#define FROGGER_STRUCTURES_H

#define GAME_MAX_NAME 32

enum entities {
    FROG,
    CAR,
    TRUCK,
    LOG,
    ENEMY_FROG,
    ENEMY_BIRD,
    ENEMY_SNAKE,
    PROJECTILES
};

/*
 * Per la rana esistono 5 diversi status in cui può trovarsi:
 * COMMON è lo stato base, lo stato in cui si trova di solito la rana.
 * POISON_COVERED è lo stato in cui ogni nemico che la attacca muore, ed è invulnerabile a essi.
 * ROAD_HAZARD è uno stato simile a POISON_COVERED, solo che al posto di essere invulnerabile ai
 * nemici è invulnerabile alle auto e ai camion.
 *
 * CONFUSED è uno stato in cui i comandi della rana sono invertiti:
 * il tasto destro la manda a sinistra, il tasto su la manda giù, ecc...
 * (Per entrare in questo stato la rana deve essere colpita da un proiettile di una rana nemica,
 * tuttavia vi è solo una chance del 25% di entrare in questo stato.)
 *
 * POISON_HURTED è uno stato in cui la rana prende 1 danno ogni tick, questo riduce
 * drammaticamente il tempo che la rana ha per arrivare dall'altra parte.
 * (Per entrare in questo stato la rana deve essere colpita dal proiettile di un serpente nemico.
 * Anche in questo caso la chance di essere afflitti dal veleno è del 25%.)
 */
enum frog_statuses {
    // GOOD statuses
    COMMON,
    POISON_COVERED,
    ROAD_HAZARD,
    // BAD statuses
    CONFUSED,
    POISON_HURTED
};

/*
 * Queste sono le modalità di gioco che ho pensato:
 * Dalla easy alla hard rispecchiano in tutto e per tutto (circa)
 * le specifiche base del progetto, la classic è la versione che rispecchia invece il gioco
 * classico, mentre la impossible è una modalità che ti ricompensa con grandi quantità
 * di monete e punti, ma per poter essere giocata necessità i powerups e tempismo.
 * La modalità impossible possiamo definirla (come piace ad Antonio) la modalità ONESHOT.
 */
enum difficulty {
    EASY,
    NORMAL,
    HARD,
    IMPOSSIBLE,
    CLASSIC
};

/*
 * Questa struttura ci consente di tenere traccia dei
 * progressi del giocatore durante ogni fase di gioco,
 * in questo modo possiamo implementare la struttura
 * 'shop' definita successivamente.
 */
typedef struct {
    int points;
    int coins;
    char name[GAME_MAX_NAME];
    enum difficulty diff;

} game;

/*
 * Questi sono oggetti comprabili con i soldi guadagnati dal superamento dei livelli.
 * I soldi sono ottenuti nel seguente modo: superando un livello al giocatore viene dato un
 * quantitativo di soldi pari al tempo medio di superamento per tutte e 5 le rane diviso 2
 * + un bonus di due monete per ogni nemico o veicolo eliminato in ogni fase del livello.
 *
 * Per quanto riguarda la modalità impossibile il conteggio è diverso:
 * Si ha a disposizione una sola rana che deve percorrere il tragitto e una sola tana in cui
 * entrare. I soldi vengono calcolati sulla base dei nemici uccisi + i veicoli distrutti +
 * 100 monete nel caso di vittoria.
 * (I veicoli alla distruzione danno al giocatore 5 monete, mentre i nemici 2.)
 */
enum shop {
    GAME_MODE_SWITCH = 100,
    POISON_COVERED_STATUS = 25,
    ROAD_HAZARD_STATUS = 25,
    SURF_ABILITY = 25,
    // NEW
    POISONED_PROJECTILE_ABILITY = 25,
    FAST_PROJECTILE_ABILITY = 25,
    EXPLOSIVE_PROJECTILE_ABILITY = 50,
    NUCLEAR_PROJECTILE_ABILITY = 100
};

struct projectile {
    enum entities entity;
    enum ability {
        POISONED_PROJECTILE,
        FAST_PROJECTILE,
        EXPLOSIVE_PROJECTILE,
        NUCLEAR_PROJECTILE
    } *abilities;
    unsigned abilities_num;
};

struct frog {
    enum frog_statuses *statuses;
    unsigned statuses_num;
    int hp;
};

struct log {
    unsigned length;
    unsigned *spaces;
    enum entities *onLog;
};

#endif //FROGGER_STRUCTURES_H
