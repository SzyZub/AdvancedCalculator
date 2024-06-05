#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#define DISP_W 500
#define DISP_H 720
#define FRAMERATE 60
#define BASEBUTTCOL al_map_rgb(230, 230, 230) 
#define DARKERBUTTCOL al_map_rgb(190, 190, 190) 

ALLEGRO_BITMAP* allBuffer;
ALLEGRO_DISPLAY* allDisplay;
ALLEGRO_TIMER* allTimer;
ALLEGRO_EVENT_QUEUE* allQueue;
ALLEGRO_EVENT allEvent;
ALLEGRO_TIMER* allTimer;
ALLEGRO_FONT* font;

typedef struct MouseInteract {
    int x, y;
    bool pressed;
}MouseInteract;

void InitTest(bool testRes, char* name)
{
    if (testRes) {
        return;
    }
    fprintf(stderr, "couldn't initialize %s\n", name);
    exit(-2);
}
void InitAllegroVars()
{
    InitTest(al_init(), "allegro");
    InitTest(al_init_primitives_addon(), "primitives addon");
    InitTest(al_init_font_addon(), "font addon");
    InitTest(al_init_ttf_addon(), "ttf");
    InitTest(al_install_mouse(), "mouse");
    font = al_load_ttf_font("font.ttf", DISP_W / 18, 0);
    InitTest(font, "font.ttf");
    allBuffer = al_create_bitmap(DISP_W, DISP_H);
    InitTest(allBuffer, "bitmap allBuffer");
    allDisplay = al_create_display(DISP_W, DISP_H);
    InitTest(allDisplay, "allDisplay");
    allTimer = al_create_timer(1.0 / FRAMERATE);
    InitTest(allTimer, "allTimer");
    allQueue = al_create_event_queue();
    InitTest(allQueue, "allQueue");
    al_register_event_source(allQueue, al_get_mouse_event_source());
    al_register_event_source(allQueue, al_get_display_event_source(allDisplay));
    al_register_event_source(allQueue, al_get_timer_event_source(allTimer));
    al_start_timer(allTimer);
}

void EventHandler(bool* reDraw, MouseInteract *str) {
    al_wait_for_event(allQueue, &allEvent);
    switch (allEvent.type) {
    case ALLEGRO_EVENT_TIMER:
        *reDraw = true;
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        exit(-1);
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        if (allEvent.mouse.button == 1) {
            (*str).x = allEvent.mouse.x;
            (*str).y = allEvent.mouse.y;
            (*str).pressed = true;
        }
        break;
    }
}

void DrawRectangle(int x, int y, ALLEGRO_COLOR col, char label[10]) {
    al_draw_filled_rectangle(x, y, x + 100, y + 80, col);
    al_draw_rectangle(x, y, x + 100, y + 80, al_map_rgb(210, 210, 210), 5);
    al_draw_text(font, al_map_rgb(10, 10, 10), x + 50, (2 * y + 80 - al_get_font_line_height(font))/2, ALLEGRO_ALIGN_CENTRE, label);
}

void DrawAnyLayer() {
    al_draw_filled_rectangle(0, 0, 600, 240, al_map_rgb(245, 245, 245));
    DrawRectangle(100, 640, BASEBUTTCOL, "*-1");
    DrawRectangle(200, 640, BASEBUTTCOL, "0");
    DrawRectangle(300, 640, BASEBUTTCOL, ",");
    DrawRectangle(400, 640, al_map_rgb(135, 200, 240), "=");
    DrawRectangle(100, 560, BASEBUTTCOL, "1");
    DrawRectangle(200, 560, BASEBUTTCOL, "2");
    DrawRectangle(300, 560, BASEBUTTCOL, "3");
    DrawRectangle(100, 480, BASEBUTTCOL, "4");
    DrawRectangle(200, 480, BASEBUTTCOL, "5");
    DrawRectangle(300, 480, BASEBUTTCOL, "6");
    DrawRectangle(100, 400, BASEBUTTCOL, "7");
    DrawRectangle(200, 400, BASEBUTTCOL, "8");
    DrawRectangle(300, 400, BASEBUTTCOL, "9");
    DrawRectangle(400, 240, BASEBUTTCOL, "Del");
    DrawRectangle(300, 240, BASEBUTTCOL, "Clear");
    DrawRectangle(200, 240, BASEBUTTCOL, "3/3");
    DrawRectangle(100, 240, BASEBUTTCOL, "2/3");
    DrawRectangle(0, 240, BASEBUTTCOL, "1/3");
}

void DrawFirstLayer() {
    DrawRectangle(0, 320, DARKERBUTTCOL, "^2");
    DrawRectangle(0, 400, DARKERBUTTCOL, "ln");
    DrawRectangle(0, 480, DARKERBUTTCOL, "log");
    DrawRectangle(0, 560, DARKERBUTTCOL, "2^x");
    DrawRectangle(0, 640, DARKERBUTTCOL, "10^x");
    DrawRectangle(100, 320, DARKERBUTTCOL, "^x");
    DrawRectangle(200, 320, DARKERBUTTCOL, "root2");
    DrawRectangle(300, 320, DARKERBUTTCOL, "rootx");
    DrawRectangle(400, 560, DARKERBUTTCOL, "+");
    DrawRectangle(400, 480, DARKERBUTTCOL, "-");
    DrawRectangle(400, 400, DARKERBUTTCOL, "*");
    DrawRectangle(400, 320, DARKERBUTTCOL, "/");
}

void DrawSecondLayer() {
    DrawRectangle(0, 320, DARKERBUTTCOL, "pi");
    DrawRectangle(0, 400, DARKERBUTTCOL, "e");
    DrawRectangle(0, 480, DARKERBUTTCOL, "|x|");
    DrawRectangle(0, 560, DARKERBUTTCOL, "exp");
    DrawRectangle(0, 640, DARKERBUTTCOL, "x!");
    DrawRectangle(100, 320, DARKERBUTTCOL, "1/x");
    DrawRectangle(200, 320, DARKERBUTTCOL, "mod");
    DrawRectangle(300, 320, DARKERBUTTCOL, "sin");
    DrawRectangle(400, 560, DARKERBUTTCOL, "cos");
    DrawRectangle(400, 480, DARKERBUTTCOL, "tg");
    DrawRectangle(400, 400, DARKERBUTTCOL, "ctg");
    DrawRectangle(400, 320, DARKERBUTTCOL, "rand");
}

void DrawThirdLayer() {
    DrawRectangle(0, 320, DARKERBUTTCOL, "floor");
    DrawRectangle(0, 400, DARKERBUTTCOL, "ceiling");
    DrawRectangle(0, 480, DARKERBUTTCOL, "appr");
    DrawRectangle(0, 560, DARKERBUTTCOL, "<<");
    DrawRectangle(0, 640, DARKERBUTTCOL, ">>");
    DrawRectangle(100, 320, DARKERBUTTCOL, "To Bin");
    DrawRectangle(200, 320, DARKERBUTTCOL, "to Oct");
    DrawRectangle(300, 320, DARKERBUTTCOL, "to Hex");
    DrawRectangle(400, 560, DARKERBUTTCOL, "And");
    DrawRectangle(400, 480, DARKERBUTTCOL, "Or");
    DrawRectangle(400, 400, DARKERBUTTCOL, "Xor");
    DrawRectangle(400, 320, DARKERBUTTCOL, "Xnor");
}

void MainDraw(bool *reDraw, short int *layer) {
    al_set_target_bitmap(allBuffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    DrawAnyLayer();
    switch (*layer) {
    case 0:
        DrawFirstLayer();
        break;
    case 1:
        DrawSecondLayer();
        break;
    case 2:
        DrawThirdLayer();
        break;
    }
    al_set_target_backbuffer(allDisplay);
    al_draw_scaled_bitmap(allBuffer, 0, 0, DISP_W, DISP_H, 0, 0, DISP_W, DISP_H, 0);
    al_flip_display();
    reDraw = false;
}

void HandleMouse(MouseInteract* mouse, short int *layer) {
    if ((*mouse).y > 240 && (*mouse).y < 320) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {
            *layer = 0;
        } else if ((*mouse).x > 100 && (*mouse).x < 200) {
            *layer = 1;
        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {
            *layer = 2;
        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    else if ((*mouse).y > 320 && (*mouse).y < 400) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {

        }
        else if ((*mouse).x > 100 && (*mouse).x < 200) {

        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {

        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    else if ((*mouse).y > 400 && (*mouse).y < 480) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {

        }
        else if ((*mouse).x > 100 && (*mouse).x < 200) {

        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {

        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    else if ((*mouse).y > 480 && (*mouse).y < 560) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {

        }
        else if ((*mouse).x > 100 && (*mouse).x < 200) {

        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {

        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    else if ((*mouse).y > 560 && (*mouse).y < 640) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {

        }
        else if ((*mouse).x > 100 && (*mouse).x < 200) {

        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {

        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    else if ((*mouse).y > 640 && (*mouse).y < 720) {
        if ((*mouse).x > 0 && (*mouse).x < 100) {

        }
        else if ((*mouse).x > 100 && (*mouse).x < 200) {

        }
        else if ((*mouse).x > 200 && (*mouse).x < 300) {

        }
        else if ((*mouse).x > 300 && (*mouse).x < 400) {

        }
        else if ((*mouse).x > 400 && (*mouse).x < 500) {

        }
    }
    (*mouse).pressed = false;
}

int main() {
    InitAllegroVars();
    bool reDraw = false;
    short int layer = 0;
    MouseInteract ms;
    ms.pressed = 0;
    while (true) {
        EventHandler(&reDraw, &ms);
        if (reDraw && al_is_event_queue_empty(allQueue)) {
            MainDraw(&reDraw, &layer);
            if (ms.pressed == 1) {
                HandleMouse(&ms, &layer);
            }
        }
    }
}