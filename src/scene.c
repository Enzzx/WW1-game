#include "libs.h"
#include "scene.h"

void createScenes (Scene *arr) {
    FILE *file;
    char line[1024];

    file = fopen("../assets/scenes.txt", "r");

    int i = 0;
    while(fgets(line, sizeof(line), file)) {
        arr[i].imgPath = malloc(32 * sizeof(char));
        sprintf(arr[i].imgPath, "../assets/scenes/scene%d.jpg", i+1);

        line[strcspn(line, "\n")] = 0;
        arr[i].text = strdup(line);
        arr[i].visibility = 0;
        arr[i].closing = false;
        i++;
    }

    fclose(file);
}