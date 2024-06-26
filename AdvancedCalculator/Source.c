#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

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
ALLEGRO_FONT* bigFont;

typedef enum TwoArg {
    Pow = 1,
    Mod,
    Root,
    Div, 
    Mult,
    Xor,
    Sub,
    Or,
    ShiftL,
    Add,
    And,
    ShiftR
} TwoArg;

typedef struct MouseInteract {
    int x, y;
    bool pressed;
    short int layer;
}MouseInteract;

typedef struct Calc {
    double a, b;
    short int fracCountA, CountA, fracCountB, CountB;
    bool show, fracMode;
    TwoArg label;
}Calc;

Calc InitCalc() {
    Calc calcValues;
    calcValues.a = calcValues.b = 0;
    calcValues.fracCountA = calcValues.fracCountB = calcValues.CountA = calcValues.CountB = 0;
    calcValues.show = calcValues.fracMode = false;
    return calcValues;
}
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
    bigFont = al_load_ttf_font("font.ttf", DISP_W / 11, 0);
    InitTest(bigFont, "font.ttf");
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
void retLabel(TwoArg label, char dest[10]) {
    switch (label) {
    case Pow:
        strcpy_s(dest, 10, "^");
        break;
    case Mod:
        strcpy_s(dest, 10, "%");
        break;
    case Root:
        strcpy_s(dest, 10, "Root");
        break;
    case Div:
        strcpy_s(dest, 10,  "/");
        break;
    case Mult:
        strcpy_s(dest, 10,  "*");
        break;
    case Xor:
        strcpy_s(dest, 10, "Xor");
        break;
    case Sub:
        strcpy_s(dest, 10,  "-");
        break;
    case Or:
        strcpy_s(dest, 10, "Or");
        break;
    case ShiftL:
        strcpy_s(dest, 10, "<<");
        break;
    case Add:
        strcpy_s(dest, 10,  "+");
        break;
    case And:
        strcpy_s(dest, 10, "And");
        break;
    case ShiftR:
        strcpy_s(dest,  10, ">>");
        break;
    }
}
void DrawAnyLayer(Calc calcValues) {
    al_draw_filled_rectangle(0, 0, 600, 240, al_map_rgb(245, 245, 245));
    DrawRectangle(100, 640, BASEBUTTCOL, "*-1");
    DrawRectangle(200, 640, BASEBUTTCOL, "0");
    DrawRectangle(300, 640, BASEBUTTCOL, "Frc/Int");
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
    if (calcValues.CountA < 15) {
        al_draw_textf(bigFont, al_map_rgb(10, 10, 10), 486, 140, ALLEGRO_ALIGN_RIGHT, "%.*f", calcValues.fracCountA, calcValues.a);
    }
    else {
        al_draw_textf(bigFont, al_map_rgb(10, 10, 10), 486, 140, ALLEGRO_ALIGN_RIGHT, "%g", calcValues.a);
    }
    if (calcValues.show) {
        if (calcValues.CountB < 15) {
            al_draw_textf(font, al_map_rgb(10, 10, 10), 486, 70, ALLEGRO_ALIGN_RIGHT, "%.*f", calcValues.fracCountB, calcValues.b);
        }
        else {
            al_draw_textf(font, al_map_rgb(10, 10, 10), 486, 70, ALLEGRO_ALIGN_RIGHT, "%g", calcValues.b);
        }
        char lab[10] = { '\0'};
        retLabel(calcValues.label, lab);
        for (int i = 0; i < 10 && lab[i] != '\0'; i++) {
            al_draw_textf(font, al_map_rgb(10, 10, 10), 30 + 17 * i, 70, ALLEGRO_ALIGN_RIGHT, "%c", lab[i]);
        }
    }
    }
void DrawFirstLayer() {
    DrawRectangle(0, 320, DARKERBUTTCOL, "^2");
    DrawRectangle(0, 400, DARKERBUTTCOL, "ln");
    DrawRectangle(0, 480, DARKERBUTTCOL, "log10");
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
    DrawRectangle(0, 560, DARKERBUTTCOL, "phi");
    DrawRectangle(0, 640, DARKERBUTTCOL, "x!");
    DrawRectangle(100, 320, DARKERBUTTCOL, "1/x");
    DrawRectangle(200, 320, DARKERBUTTCOL, "%");
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
    DrawRectangle(300, 320, DARKERBUTTCOL, "e^");
    DrawRectangle(400, 560, DARKERBUTTCOL, "And");
    DrawRectangle(400, 480, DARKERBUTTCOL, "Or");
    DrawRectangle(400, 400, DARKERBUTTCOL, "Xor");
    DrawRectangle(400, 320, DARKERBUTTCOL, "Not");
}
void MainDraw(bool *reDraw, short int layer, Calc calcValues) {
    al_set_target_bitmap(allBuffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    DrawAnyLayer(calcValues);
    switch (layer) {
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
    *reDraw = false;
}
void ExpandNumb(Calc *calcValues, int num) {
    if (calcValues->CountA < 14) {
        if (calcValues->a >= 0) {
            if (!calcValues->fracMode) {
                long long temp = (long long)calcValues->a;
                calcValues->a -= temp;
                temp *= 10;
                temp += num;
                calcValues->a += temp;
                calcValues->CountA++;
            }
            else {
                calcValues->fracCountA++;
                calcValues->a += num * (1.0 / pow(10, calcValues->fracCountA));
                calcValues->CountA++;
            }
        }
    }
}
long long CountInts(Calc calcValues) {
    int count = 0;  
    long long value = calcValues.a;
    while (value != 0)
    {
        value = value / 10;
        count++;
    }
    return count;
} 
void PutToB(Calc* calcValues, TwoArg label) {
    calcValues->show = true;
    calcValues->fracMode = false;
    calcValues->b = calcValues->a;
    calcValues->CountB = calcValues->CountA;
    calcValues->fracCountB = calcValues->fracCountA;
    calcValues->a = 0;
    calcValues->CountA = 0;
    calcValues->fracCountA = 0;
    calcValues->label = label;
}
void HandleEquals(Calc* calcValues) {
    switch (calcValues->label) {
    case Pow:
        calcValues->a = pow(calcValues->b, calcValues->a);
        calcValues->fracCountA = 8;
        break;
    case Mod:
        if (calcValues->a >= 0.000000001 || calcValues->a <= -0.000000001) {
            calcValues->a = (long long)calcValues->b % (long long)calcValues->a;          
        }
        else {
            calcValues->a = NAN;
        }
        calcValues->fracCountA = 0;
        break;
    case Root:
        calcValues->a = pow(calcValues->b, 1/calcValues->a);
        calcValues->fracCountA = 8;
        break;
    case Div:
        calcValues->a = calcValues->b / calcValues->a;
        calcValues->fracCountA = 8;
        break;
    case Mult:
        calcValues->a = calcValues->b * calcValues->a;
        calcValues->fracCountA = 8;
        break;
    case Xor:
        if (calcValues->a == (long long)calcValues->a && calcValues->b == (long long)calcValues->b) {
            calcValues->a = (long long) calcValues->b ^ (long long) calcValues->a;
        }
        else {
            calcValues->a = NAN;
        }
        calcValues->fracCountA = 0;
        break;
    case Sub:
        calcValues->a = calcValues->b - calcValues->a;
        calcValues->fracCountA = max(calcValues->fracCountA, calcValues->fracCountB);
        break;
    case Or:
        if (calcValues->a == (long long)calcValues->a && calcValues->b == (long long)calcValues->b) {
            calcValues->a = (long long)calcValues->b | (long long)calcValues->a;
        }
        else {
            calcValues->a = NAN;
        }
        calcValues->fracCountA = 0;
        break;
    case ShiftL:
        if (calcValues->a == (long long)calcValues->a && calcValues->b == (long long)calcValues->b) {
            calcValues->a = (long long)calcValues->b << (long long)calcValues->a;
        }
        else {
            calcValues->a = NAN;
        }
        break;
    case Add:
        calcValues->a = calcValues->b + calcValues->a;
        calcValues->fracCountA = max(calcValues->fracCountA, calcValues->fracCountB);
        break;
    case And:
        if (calcValues->a == (long long)calcValues->a && calcValues->b == (long long)calcValues->b) {
            calcValues->a = (long long)calcValues->b & (long long)calcValues->a;
        }
        else {
            calcValues->a = NAN;
        }
        calcValues->fracCountA = 0;
        break;
    case ShiftR:
        if (calcValues->a == (long long)calcValues->a && calcValues->b == (long long)calcValues->b) {
            calcValues->a = (long long)calcValues->b >> (long long)calcValues->a;
        }
        else {
            calcValues->a = NAN;
        }
        break;
    default:
        return;
    }
    calcValues->b = 0;
    calcValues->CountB = 0;
    calcValues->fracCountB = 0;
    calcValues->fracMode = false;
    calcValues->show = false;
}
void HandleMouse(MouseInteract* mouse, Calc* calcValues) {
    int twoArgs = 0;
    if (mouse->y > 240 && mouse->y < 320) {
        if (mouse->x > 0 && mouse->x < 100) {
            twoArgs = 1;
            mouse->layer = 0;
        } else if (mouse->x > 100 && mouse->x < 200) {
            twoArgs = 1;
            mouse->layer = 1;
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            twoArgs = 1;
            mouse->layer = 2;
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            *calcValues = InitCalc();
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            twoArgs = 1;
            double intPart, fract;
            fract = modf(calcValues->a, &intPart);
            if (calcValues->a != 0) { 
                if (calcValues->fracMode) {
                    if (calcValues->fracCountA) {
                        calcValues->fracCountA--;
                        long long frac_as_int = (long long)(fract * (10 * pow(10, calcValues->fracCountA)));
                        frac_as_int /= 10;
                        fract = frac_as_int / (10 * pow(10, calcValues->fracCountA - 1));
                    }
                }
                else {
                    long long int_as_int = (long long) intPart;
                    int_as_int /= 10;
                    intPart = int_as_int;
                }
                calcValues->a = fract + intPart;
            }
        }
    }
    else if (mouse->y > 320 && mouse->y < 400) {
        if (mouse->x > 0 && mouse->x < 100) {
            if (mouse->layer == 0) {
                calcValues->a *= calcValues->a;
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1) {
                calcValues->a = 3.14159265;
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else {
                calcValues->fracCountA = 0;
                calcValues->fracMode = false;
                calcValues->a = floor(calcValues->a);
            }
        } 
        else if (mouse->x > 100 && mouse->x < 200) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Pow);
            }
            else if (mouse->layer == 1) {
                calcValues->a = 1 / calcValues->a;
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else {
                if (calcValues->a == (long long)calcValues->a) {
                    long long temp = (long long)calcValues->a, result = 0, pos = 1;
                    while (temp)
                    {
                        result += (temp % 2) * pos;
                        temp /= 2;
                        pos *= 10;
                    }
                    calcValues->a = result;
                    calcValues->CountA = CountInts(*calcValues);
                    calcValues->fracMode = false;
                }
            }
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            if (mouse->layer == 0) {
                calcValues->a = sqrt(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1) {
                twoArgs = 1;
                PutToB(calcValues, Mod);
            }
            else {
                if (calcValues->a == (long long)calcValues->a) {
                    long long temp = (long long) calcValues->a, result = 0, pos = 1;
                    while (temp)
                    {
                        result += (temp % 8) * pos;
                        temp /= 8;
                        pos *= 10;
                    }
                    calcValues->a = result;
                    calcValues->fracMode = false;
                }
            }
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Root);
            }
            else if (mouse->layer == 1) {
                calcValues->a = sin(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else {
                calcValues->a = exp(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Div);
            }
            else if (mouse->layer == 1) {
                calcValues->a = rand();
                calcValues->fracMode = false;
            }
            else {
                calcValues->a = ~ (long long) calcValues->a;
                calcValues->fracMode = false;
            }
        }
    }
    else if (mouse->y > 400 && mouse->y < 480) {
        if (mouse->x > 0 && mouse->x < 100) {
            if (mouse->layer == 0) {
                calcValues->a = log(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1) {
                calcValues->a = 2.71828182;
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else {
                calcValues->fracCountA = 0;
                calcValues->fracMode = false;
                calcValues->a = ceil(calcValues->a);
            }
        }
        else if (mouse->x > 100 && mouse->x < 200) {
            twoArgs = 1;
            ExpandNumb(calcValues, 7);
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            twoArgs = 1;
            ExpandNumb(calcValues, 8);
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            twoArgs = 1;
            ExpandNumb(calcValues, 9);
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Mult);
            }
            else if (mouse->layer == 1) {
                if (calcValues->a != 3.14159265) {
                    calcValues->a = 1 / tan(calcValues->a);
                    calcValues->fracCountA = 8;
                    calcValues->fracMode = false;
                }
                else {
                    calcValues->a = NAN;
                    calcValues->fracCountA = 0;
                    calcValues->fracMode = false;
                }
               
            }
            else if (mouse->layer == 2){
                twoArgs = 1;
                PutToB(calcValues, Xor);
            }
        }
    }
    else if (mouse->y > 480 && mouse->y < 560) {
        if (mouse->x > 0 && mouse->x < 100) {
            if (mouse->layer == 0) {
                calcValues->a = log10(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1) {
                calcValues->a = fabs(calcValues->a);
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 2) {
                calcValues->fracCountA = 0;
                calcValues->fracMode = false;
                calcValues->a = round(calcValues->a);
            }
        }
        else if (mouse->x > 100 && mouse->x < 200) {
            twoArgs = 1;
            ExpandNumb(calcValues, 4);
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            twoArgs = 1;
            ExpandNumb(calcValues, 5);
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            twoArgs = 1;
            ExpandNumb(calcValues, 6);
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Sub);
            }
            else if (mouse->layer == 1) {
                calcValues->a = tan(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 2) {
                twoArgs = 1;
                PutToB(calcValues, Or);
            }
        }
    }
    else if (mouse->y > 560 && mouse->y < 640) {
        if (mouse->x > 0 && mouse->x < 100) {
            if (mouse->layer == 0) {
                calcValues->a = pow(2, calcValues->a);
                if (calcValues->a != (long long)calcValues->a) {
                    calcValues->fracCountA = 8;
                }
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1) {
                calcValues->a = 1.61803398;
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 2){
                twoArgs = 1;
                PutToB(calcValues, ShiftL);
            }
        }
        else if (mouse->x > 100 && mouse->x < 200) {
            twoArgs = 1;
            ExpandNumb(calcValues, 1);
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            twoArgs = 1;
            ExpandNumb(calcValues, 2);
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            twoArgs = 1;
            ExpandNumb(calcValues, 3);
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            if (mouse->layer == 0) {
                twoArgs = 1;
                PutToB(calcValues, Add);
            }
            else if (mouse->layer == 1) {
                calcValues->a = cos(calcValues->a);
                calcValues->fracCountA = 8;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 2) {
                twoArgs = 1;
                PutToB(calcValues, And);
            }
        }
    }
    else if (mouse->y > 640 && mouse->y < 720) {
        if (mouse->x > 0 && mouse->x < 100) {
            if (mouse->layer == 0) {
                calcValues->a = pow(10, calcValues->a);
                if (calcValues->a != (long long)calcValues->a) {
                    calcValues->fracCountA = 8;
                }
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 1 && calcValues->a >= 0) {
                double fact = 1;
                for (double c = 1; c <= (long long)calcValues->a; c++) {
                    fact = fact * c;
                }
                calcValues->a = fact;
                calcValues->fracCountA = 0;
                calcValues->fracMode = false;
            }
            else if (mouse->layer == 2) {
                twoArgs = 1;
                PutToB(calcValues, ShiftR);
            }
        }
        else if (mouse->x > 100 && mouse->x < 200) {
            calcValues->a *= -1;
            calcValues->fracMode = false;
        }
        else if (mouse->x > 200 && mouse->x < 300) {
            if (calcValues->CountA < 14) {
                twoArgs = 1;
                if (!calcValues->fracMode && calcValues->a != 0) {
                    long long temp = (long long)calcValues->a;
                    calcValues->a -= temp;
                    temp *= 10;
                    calcValues->a += temp;
                }
                else if (calcValues->fracMode){
                    calcValues->fracCountA++;
                }
            }
        }
        else if (mouse->x > 300 && mouse->x < 400) {
            twoArgs = 1;
            calcValues->fracMode = !calcValues->fracMode;
        }
        else if (mouse->x > 400 && mouse->x < 500) {
            HandleEquals(calcValues);
        }
    }
    if (!twoArgs) {
        calcValues->b = 0;
        calcValues->CountB = 0;
        calcValues->fracCountB = 0;
        calcValues->fracMode = false;
        calcValues->show = false;
    }
    if (isinf(calcValues->a) || isnan(calcValues->a)) {
        calcValues->a = NAN;
    }
    calcValues->CountA = CountInts(*calcValues) + calcValues->fracCountA;
    mouse->pressed = false;
}
int main() {
    srand(time(0));
    InitAllegroVars();
    bool reDraw = false;
    MouseInteract ms;
    ms.pressed = ms.layer = 0;
    Calc calcValues = InitCalc();
    while (true) {
        EventHandler(&reDraw, &ms);
        if (reDraw && al_is_event_queue_empty(allQueue)) {
            MainDraw(&reDraw, ms.layer, calcValues);
            if (ms.pressed == 1) {
                HandleMouse(&ms, &calcValues);
            }
        }
    }
}