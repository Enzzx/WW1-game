#include "libs.h"
#include "scene.h"
#include "gamestate.h"
#include "level.h"


int WIDTH = 700;
int HEIGHT = 450;
float FPS = 30;
GAMESTATE stateCurrent = STATE_MENU;
Scene scenesArr[9];
Stage stagesArr[5];
int indeXcene = 0;
int indeXtage = 0;
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
    createLevels(stagesArr);

    resizeBg("../assets/menu.jpeg", background, WIDTH, HEIGHT);

    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == fps) {
            redraw = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        switch (stateCurrent) {
            case STATE_MENU:
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_text(font, al_map_rgb(255, 255, 255), 150, 0, ALLEGRO_ALIGN_CENTRE, "APERTE ENTER PRA INICIAR O JOGO");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    stateCurrent = STATE_SCENE;
                    resizeBg(scenesArr[indeXcene].imgPath, background, WIDTH, HEIGHT);
                }
                break;

            case STATE_SCENE:
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH/2, 0, ALLEGRO_ALIGN_CENTRE, scenesArr[indeXcene].text);
                
                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode ==
                 ALLEGRO_KEY_RIGHT && scenesArr[indeXcene].visibility >= 1.0)
                    scenesArr[indeXcene].closing = true;
                break;

            case STATE_LEVEL:
                if (redraw) {
                    if(!(stagesArr[indeXtage].ingame || stagesArr[indeXtage].closing)) { // PRÓLOGO
                        if (!(stagesArr[indeXtage].opened) && stagesArr[indeXtage].visibility < 1) {
                            fadeState(&stagesArr[indeXtage].visibility, false, FPS);
                        } else if (stagesArr[indeXtage].opened) {
                            if(fadeState(&stagesArr[indeXtage].visibility, true, FPS))
                                stagesArr[indeXtage].opened = false;
                                stagesArr[indeXtage].ingame = true;
                        }

                        al_draw_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(0, 0, 0), 0);
                        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, stagesArr[indeXtage].prologue);

                        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode ==  ALLEGRO_KEY_RIGHT)
                            stagesArr[indeXtage].opened = true;;

                    } else if (stagesArr[indeXtage].ingame) { // JOGO
                        if (!(stagesArr[indeXtage].opened)) {
                            fadeState(&stagesArr[indeXtage].visibility, false, FPS);
                            if (stagesArr[indeXtage].visibility >= 1)
                                stagesArr[indeXtage].opened = true;
                        } else if (stagesArr[indeXtage].opened && stagesArr[indeXtage].visibility < 1) {
                            if (fadeState(&stagesArr[indeXtage].visibility, true, FPS)) {
                                stagesArr[indeXtage].opened = false;
                                stagesArr[indeXtage].ingame = false;
                                stagesArr[indeXtage].closing = true;
                            }
                        }
                        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) 
                            stateCurrent = STATE_PAUSE;


                        al_draw_bitmap(background, 0, 0, 0);
                        // CHAMADA DO JOGO
                        // EVENTO PRA FADEOUT E MUDAR PRO EPILOGO
                    } else if (stagesArr[indeXtage].closing) { // EPÍLOGO
                        if (!(stagesArr[indeXtage].opened) && stagesArr[indeXtage].visibility < 1)
                            fadeState(&stagesArr[indeXtage].visibility, false, FPS);

                        al_draw_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(0, 0, 0), 0);
                        al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, stagesArr[indeXtage].epilogue);

                        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                            stagesArr[indeXtage].opened = true;
                    }
                }
                break;

            case STATE_GAMEOVER:
                al_draw_text(font, al_map_rgb(255, 255, 255), 250, 0, 0, "game over");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    stateCurrent = STATE_MENU;
                break;

            case STATE_PAUSE:
                al_draw_text(font, al_map_rgb(255, 255, 255), 250, 0, 0, "pausado");

                if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    stateCurrent = STATE_LEVEL;
                }
                break;
        
            default:
                printf("enum tá com problema");
                break;
        }


        if (redraw && al_is_event_queue_empty(queue)) {
            if (stateCurrent == STATE_SCENE && (scenesArr[indeXcene].visibility != 1 || scenesArr[indeXcene].closing == true)) {
                if (fadeState(&scenesArr[indeXcene].visibility, scenesArr[indeXcene].closing, FPS)) {
                    indeXcene++;
                    if (indeXcene == stagesArr[indeXtage].afterScene) {
                        stateCurrent = STATE_LEVEL;
                        resizeBg(stagesArr[indeXtage].mapPath, background, WIDTH, HEIGHT);
                    } else {
                    resizeBg(scenesArr[indeXcene].imgPath, background, WIDTH, HEIGHT);
                    }
                }
            } else if (stateCurrent == STATE_LEVEL && stagesArr[indeXtage].opened && stagesArr[indeXtage].closing) {
                if (fadeState(&stagesArr[indeXtage].visibility, stagesArr[indeXtage].closing, FPS)) {
                    indeXtage++;
                    resizeBg(scenesArr[indeXcene].imgPath, background, WIDTH, HEIGHT);
                    stateCurrent = STATE_SCENE;
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
    al_destroy_timer(fps);
    al_destroy_event_queue(queue);

    return 0;
}