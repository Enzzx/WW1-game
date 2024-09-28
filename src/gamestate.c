#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/gamestate.h"

void resizeBg(char *path, ALLEGRO_BITMAP *background, int width, int height) {
    ALLEGRO_BITMAP *defaultBg = al_load_bitmap(path);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();
    if (!defaultBg) {
        printf("Erro ao carregar imagem: %s\n", path);
    }

    al_set_target_bitmap(background);
    al_draw_scaled_bitmap(defaultBg, 0, 0, al_get_bitmap_width(defaultBg), al_get_bitmap_height(defaultBg), 0, 0, width, height, 0);
    al_set_target_bitmap(prev_target);

    al_destroy_bitmap(defaultBg);
}


bool fadeState(float *visibility, bool closing, int fps) {
    printf("entrou em fadestate\n");
    if (closing == true)
        *visibility -= 0.1;
    else if (*visibility < 1)
        *visibility += 0.1;
    
    if (*visibility <= 0.001 && closing == true)
        return true;
    return false;
}