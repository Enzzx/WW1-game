#ifndef LEVEL_H
#define LEVEL_H

typedef enum {
    CAMBRAI,
    SOMME,
    JUTLAND,
    VERDUN,
    SPRING
} GAME_LEVEL;

typedef struct {
    GAME_LEVEL level;
    char *prologue;
    char *epilogue;
    int afterScene;
    char *mapPath;
    float visibility;
    bool opened;
    bool ingame;
    bool closing;
} Stage;

void createLevels (Stage *arr);

#endif