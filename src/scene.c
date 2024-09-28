#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/scene.h"

void createScenes (Scene *arr) {
    FILE *file;
    char line[1000];

    file = fopen("../assets/scenes.txt", "r");

    int index = 0;
    while(fgets(line, sizeof(line), file)) {
        arr[index].imgPath = malloc(sizeof(char) * 25);
        sprintf(arr[index].imgPath, "../assets/scene%d.jpg", index+1);

        line[strcspn(line, "\n")] = 0;
        arr[index].text = strdup(line);
        arr[index].shiftX = 0;
        arr[index].visibility = 0;
        arr[index].closing = false;
        index++;
    }

    fclose(file);
}