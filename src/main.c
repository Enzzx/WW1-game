#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/scene.h"
#include "../include/gamestate.h"


typedef enum {
    STATE_MENU,
    STATE_SCENE,
    STATE_LEVEL,
    STATE_GAMEOVER,
    STATE_PAUSE
} GAMESTATE;

int WIDTH = 700;
int HEIGHT = 450;
float FPS = 5;
Scene scenesArr[2];
int indeXcene = 0;
GAMESTATE stateCurrent = STATE_MENU;
bool redraw = true;


int main()
{
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();

    ALLEGRO_TIMER *fps = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_BITMAP * background = al_create_bitmap(WIDTH, HEIGHT);
    ALLEGRO_EVENT event;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(fps));
    al_start_timer(fps);
    createScenes(scenesArr);

    ALLEGRO_TIMER *tempo = al_create_timer(4.0);
    al_register_event_source(queue, al_get_timer_event_source(tempo));


    resizeBg("../assets/menu.jpeg", background, WIDTH, HEIGHT);



    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == fps) {
            redraw = true;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;


        switch (stateCurrent) {
            case STATE_MENU:
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_text(font, al_map_rgb(255, 255, 255), 150, 0, 0, "APERTE ENTER PRA INICIAR O JOGO");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    stateCurrent = STATE_SCENE;
                    resizeBg(scenesArr[indeXcene].imgPath, background, WIDTH, HEIGHT);
                }
                break;

            case STATE_SCENE:
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH/2, 0, 0, scenesArr[indeXcene].text);
                al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba_f(0, 0, 0, 1-scenesArr[indeXcene].visibility));

                /* if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                    stateCurrent = STATE_LEVEL;
                    al_start_timer(tempo);
                } */
                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode ==
                 ALLEGRO_KEY_RIGHT && scenesArr[indeXcene].visibility >= 1.0)
                    scenesArr[indeXcene].closing = true;
                break;

            case STATE_LEVEL:
                al_draw_text(font, al_map_rgb(255, 255, 255), 250, 0, 0, "jogando");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    stateCurrent = STATE_PAUSE;
                    al_stop_timer(tempo);
                } else if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == tempo)
                    stateCurrent = STATE_GAMEOVER;
                break;

            case STATE_GAMEOVER:
                al_draw_text(font, al_map_rgb(255, 255, 255), 250, 0, 0, "game over");

                if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == tempo)
                    stateCurrent = STATE_MENU;
                break;

            case STATE_PAUSE:
                al_draw_text(font, al_map_rgb(255, 255, 255), 250, 0, 0, "pausado");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    stateCurrent = STATE_LEVEL;
                    al_start_timer(tempo);
                }
                break;
        
            default:
                printf("enum tá com problema");
                break;
        }


        if (redraw && al_is_event_queue_empty(queue)) {
            if (stateCurrent == STATE_SCENE && (scenesArr[indeXcene].visibility != 1 || scenesArr[indeXcene].closing == true)) {
                printf("entrou no redraw no estado 'scene'\nvisibilidade da cena: %.2f\nclosing: %d\n", scenesArr[indeXcene].visibility, scenesArr[indeXcene].closing);
                if (fadeState(&scenesArr[indeXcene].visibility, scenesArr[indeXcene].closing, FPS)) {
                    indeXcene++;
                    resizeBg(scenesArr[indeXcene].imgPath, background, WIDTH, HEIGHT);

                    if (indeXcene == sizeof(scenesArr)/sizeof(scenesArr[0])) {
                        stateCurrent = STATE_LEVEL;
                        al_start_timer(tempo);
                    }
                }
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            redraw = false;
        }
    }

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(tempo);
    al_destroy_timer(fps);
    al_destroy_event_queue(queue);

    return 0;
}