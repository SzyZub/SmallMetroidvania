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
    case options:
        optionsDraw(programScene);
        break;
    case edit:
        editorDraw(programScene);
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
        if (y < 192) {
            programScene = game;
            loadmap(1, 1, objList);
        }
        else if (y >= 192 && y < 372) {
            programScene = edit;
        }
        else if (y >= 372 && y < 562) {
            programScene = options;
        }
        else if (y > 562){
            programScene = ext;
        }
    }
}

void optionsDraw(EnScene& programScene) {
    DrawText("Window", (SCREENW - MeasureText("Window", 48)) / 2, 48, 48, DARKGREEN);
    DrawText("Fullscreen", (SCREENW - MeasureText("Fullscreen", 35)) / 4, 150, 35, IsWindowState(FLAG_FULLSCREEN_MODE) ? GREEN : RED);
    DrawText("Windowed", (SCREENW - MeasureText("Windowed", 35)) / 4* 3, 150, 35, IsWindowState(FLAG_FULLSCREEN_MODE) ? RED : GREEN);
    DrawText("Exit", (SCREENW - MeasureText("Exit", 48)) / 2, 672, 48, DARKGREEN);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        int x = GetMouseX();
        if (y > 120 && y < 210) {
            if (x < SCREENW / 2) {
                SetWindowState(FLAG_FULLSCREEN_MODE);
            }
            else {
                ClearWindowState(FLAG_FULLSCREEN_MODE);
                SetWindowSize(SCREENW, SCREENH);
            }
        }
        else if (y > 624){
            programScene = title;
        }
    }
}

void editorDraw(EnScene& programScene) {

}