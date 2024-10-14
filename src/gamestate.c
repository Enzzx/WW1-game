#include "libs.h"
#include "gamestate.h"

extern int WIDTH;
extern int HEIGHT;
extern ALLEGRO_FONT *font;

void resizeBg(char *path, ALLEGRO_BITMAP *background) {
    ALLEGRO_BITMAP *defaultBg = al_load_bitmap(path);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();
    if (!defaultBg) {
        printf("Erro ao carregar imagem: %s\n", path);
    }

    al_set_target_bitmap(background);
    al_draw_scaled_bitmap(defaultBg, 0, 0, al_get_bitmap_width(defaultBg), al_get_bitmap_height(defaultBg), 0, 0, WIDTH, HEIGHT, 0);
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

void textBox(int x, int y, int width, int padding, char *text) {
    char *copyText = malloc(1024 * sizeof(char));
    strcpy(copyText, text);
    char *newText = malloc(1024 * sizeof(char));
    newText[0] = '\0';
    char *line = malloc(256 * sizeof(char));
    line[0] = '\0';
    char *word = strtok(copyText, " ");
    int lineCount = 1;
    int lineHeight = al_get_font_line_height(font);
    while (word) {
        if (al_get_text_width(font, line) + al_get_text_width(font, word) >= width - padding) {
            strcat(newText, line);
            strcat(newText, "\n");
            line[0] = '\0';
            lineCount++;
        }
        strcat(line, word);
        strcat(line, " ");
        word = strtok(NULL, " ");
    }
    if (line[0] != '\0') {
        strcat(newText, line);
    }

    al_draw_filled_rectangle(x - width/2, y, x + width/2, y + lineCount*lineHeight + padding*2, al_map_rgba_f(0, 0, 0, 0.6));

    char *buffer  = strtok(newText, "\n");
    lineCount = 0;
    while (buffer) {
        al_draw_text(font, al_map_rgb(255, 255, 255), x - width/2 + padding/2, y + lineCount*lineHeight + padding, 0, buffer);
        buffer = strtok(NULL, "\n");
        lineCount++;
    }

    free(line);
    free(copyText);
    free(newText);
}