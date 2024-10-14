#include "libs.h"
#include "level.h"

void createLevels (Stage *arr) {
    FILE *file;
    char line[1024];

    file = fopen("../assets/stages.txt", "r");

    int afterScene[] = {4, 5, 6, 8, 9};
    int i = 0;
    GAME_LEVEL lvl = CAMBRAI;
    while(fgets(line, sizeof(line), file)) {
        arr[i].level = lvl;
        arr[i].prologue = malloc(512 * sizeof(char));
        strcpy(arr[i].prologue, strtok(line, "/"));
        arr[i].epilogue = malloc(512 * sizeof(char));
        strcpy(arr[i].epilogue, strtok(NULL, ""));
        arr[i].afterScene = afterScene[i];
        arr[i].mapPath = malloc(32 * sizeof(char));
        sprintf(arr[i].mapPath, "../assets/maps/map%d.webp", i+1);
        arr[i].visibility = 0;
        arr[i].opened = false;
        arr[i].ingame = false;
        arr[i].closing = false;

        lvl++;
        i++;
    }
}