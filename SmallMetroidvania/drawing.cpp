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
        static Drawer draw;
        draw.editDraw(objList, programScene);
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

void Drawer::editDraw(std::vector <Object*>& objList, EnScene& programScene) {
    if (editMode) {
        for (int i = 0; i < SCREENW; i += 32) {
            DrawLine(i, 0, i, SCREENH, BLACK);
        }
        for (int i = 0; i < SCREENH; i += 32) {
            DrawLine(0, i, SCREENW, i, BLACK);
        }
        int x = GetMouseX();
        int y = GetMouseY();
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
        if (measure) {
            DrawLineEx({ (float)x, (float)y }, { (float)x, (float)y - 105 }, 3, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x + 112, (float)y }, 3, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x - 112, (float)y }, 3, BLACK);
        }
        if (drawBlock) {
            DrawCircle(prevX, prevY, 5, RED);
            DrawLineEx({ (float) prevX, (float) prevY }, { (float) prevX, (float) prevY - 105 }, 3, BLACK);
            DrawLineEx( {(float) prevX, (float)prevY}, {(float) prevX + 112, (float) prevY}, 3, BLACK);
            DrawLineEx( {(float) prevX, (float)prevY }, {(float) prevX - 112, (float) prevY },3, BLACK);
        }
        if (IsKeyPressed(KEY_COMMA))
            measure = !measure;
        else if (IsKeyPressed(KEY_ONE))
            editMaterial = wall;
        else if (IsKeyPressed(KEY_PERIOD))
            drawBlock = false;
        else if (IsKeyPressed(KEY_U))
            if(objList.size() != 0)
                objList.pop_back();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && drawBlock == false) {
            prevX = x;
            prevY = y;
            drawBlock = true;
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && drawBlock == true) {
            switch (editMaterial) {
            case wall:
                objList.push_back(new BackgroundWall(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 2, abs(y - prevY)+2));
            }
            drawBlock = false;
        }
    }
    else {
        DrawText("Press e to toggle between helper and edit", 20, 20, 32, BLACK);
        DrawText("Press , to bring up a measure of how high player will jump", 20, 80, 32, BLACK);
        DrawText("Press . to undo pressing the mouse", 20, 140, 32, BLACK);
        DrawText("Press s to save the map", 20, 200, 32, BLACK);
        DrawText("Press g to exit without saving the map", 20, 260, 32, BLACK);
        DrawText("Press U to undo placing the recent object", 20, 320, 32, BLACK);
        DrawText("Press 1 to choose wall", 20, 380, 32, BLACK);
    }
    if (IsKeyPressed(KEY_E))
        editMode = !editMode;  
    else if (IsKeyPressed(KEY_G)) {
        programScene = title;
        editMode = false;
        drawBlock = false;
        measure = false;
        editMaterial = wall;
        prevX = prevY = 0;
        deloadmap(objList);
    }
    else if (IsKeyPressed(KEY_S)) {
        savemap(objList);
    }

}

Drawer::Drawer() {
    editMode = false;
    drawBlock = false;
    measure = false;
    editMaterial = wall;
    prevX = prevY = 0;
}