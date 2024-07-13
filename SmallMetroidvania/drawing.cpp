#include "drawing.h"

void initScreen() {
    InitWindow(SCREENW, SCREENH, "MetroidCube");
    SetTargetFPS(60);
}

void mainDraw(std::vector <Object*>& objList, EnScene& programScene) {
    BeginDrawing();
    ClearBackground(BACKGROUNDCOLOR);
    switch (programScene) {
    case game: 
        gameDraw(objList, programScene);
        break;
    case title:
        titleDraw(objList, programScene);
        break;
    }
    EndDrawing();
}

void gameDraw(std::vector <Object*>& objList, EnScene& programScene) {
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

void titleDraw(std::vector <Object*>& objList, EnScene& programScene) {
    DrawText("Start the game", (SCREENW - MeasureText("Start the game", 64))/2, 70, 64, DARKGREEN);
    DrawText("Editor", (SCREENW - MeasureText("Editor", 64)) / 2, 250, 64, DARKGREEN);
    DrawText("Options", (SCREENW - MeasureText("Options", 64)) / 2, 430, 64, DARKGREEN);
    DrawText("Exit", (SCREENW - MeasureText("Exit", 64)) / 2, 610, 64, DARKGREEN);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        if (y < 160) {
            programScene = game;
            loadmap(1, objList);
        }
        else if (y >= 160 && y < 340) {
            programScene = edit;
        }
        else if (y >= 340 && y < 520) {
            programScene = options;
        }
        else if (y > 520){
            programScene = ext;
        }
    }
}