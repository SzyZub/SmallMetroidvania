#include "drawing.h"

EnScene programScene = title;

void initScreen() {
    InitWindow(SCREENW, SCREENH, "MetroidCube");
    SetTargetFPS(60);
}

void mainDraw(std::vector <Object*>& objList) {
    BeginDrawing();
    ClearBackground(BACKGROUNDCOLOR);
    switch (programScene) {
    case game: 
        gameDraw(objList);
        break;
    case title:
        titleDraw(objList);
        break;
    }
    EndDrawing();
}

void gameDraw(std::vector <Object*>& objList) {
    objList[0]->move(objList);
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        switch ((*it)->label) {
        case enLabel::player:
            DrawRectangle((*it)->x, (*it)->y, (*it)->width, (*it)->height, PLAYERCOLOR);
            break;
        case enLabel::wall:
            DrawRectangle((*it)->x, (*it)->y, (*it)->width, (*it)->height, WALLCOLOR);
            break;
        }
    }
}

void titleDraw(std::vector <Object*>& objList) {
    DrawText("Start the game", (SCREENW - MeasureText("Start the game", 32))/2, 50, 32, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (GetMouseY() < 82) {
            programScene = game;
            loadmap(1, objList);
        }
    }
}