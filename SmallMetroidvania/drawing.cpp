#include "drawing.h"

void initScreen() {
    InitWindow(SCREENW, SCREENH, "MetroidCube");
    SetTargetFPS(50);
}

void mainDraw(std::vector <Object*> objList) {
    BeginDrawing();
    ClearBackground(BACKGROUNDCOLOR);
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        switch ((*it)->label) {
        case enLabel::player:
            DrawRectangle((*it)->x, (*it)->y, (*it)->width, (*it)->height, { 100, 100, 100, 255 });
        }
    }
    EndDrawing();
}