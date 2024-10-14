#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef enum {
    STATE_MENU,
    STATE_SCENE,
    STATE_LEVEL,
    STATE_GAMEOVER,
    STATE_PAUSE
} GAMESTATE;

void resizeBg(char *path, ALLEGRO_BITMAP *background);
bool fadeState(float *visibility, bool closing, float fps);
void textBox(int x, int y, int width, int padding, char *text);

#endif