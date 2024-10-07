#include "libs.h"
#include "gamestate.h"

extern int WIDTH;
extern int HEIGHT;

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


bool fadeState(float *visibility, bool closing, float fps) {
    if (closing == true)
        *visibility -= (1/fps)*1.2;
    else if (*visibility < 1)
        *visibility += (1/fps)*1.2;
    
    al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba_f(0, 0, 0, 1-*visibility));

    if (*visibility <= 0.001 && closing == true)
        return true;
    return false;
}