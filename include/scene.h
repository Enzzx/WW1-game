#ifndef SCENE_H
#define SCENE_H


typedef struct {
    char *imgPath;
    char *text;
    float visibility;
    bool closing;
} Scene;

void createScenes(Scene *arr);

#endif