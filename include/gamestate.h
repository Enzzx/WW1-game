#ifndef GAMESTATE_H
#define GAMESTATE_H

void resizeBg(char *path, ALLEGRO_BITMAP *background, int width, int height);

bool fadeState(float *visibility, bool closing, float fps);

#endif