#include "drawing.h"

void initScreen(GameManager GM) {
    InitWindow(GM.originalW, GM.originalH, "MetroidCube");
    SetTargetFPS(GM.framerate);
    SetExitKey(KEY_GRAVE);
}

void mainDraw(GameManager& GM, MapManager& MM) {
    BeginDrawing();
    ClearBackground(BACKGROUNDCOLOR);
    switch (GM.sceneLabel) {
    case game: 
        gameDraw(GM, MM);
        break;
    case title:
        titleDraw(GM);
        break;
    case options:
        optionsDraw(GM);
        break;
    case choosing:
        chooseMap(GM, MM);
        break;
    case menu:
        menuDraw(GM, MM);
        break;
    case edit:
        static EditorDrawer draw;
        draw.editDraw(GM, MM);
        break;
    }
    EndDrawing();
}

void chooseMap(GameManager& GM, MapManager& MM) {
    DrawText("Main Campaign", (GM.originalW-MeasureText("Main Campaign", MENUFONT))/2, GM.originalH/8-MENUFONT/2, MENUFONT, BLACK);
    DrawText("Custom Campaign", (GM.originalW - MeasureText("Custom Campaign", MENUFONT)) / 2, GM.originalH * 3/8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Test Map", (GM.originalW - MeasureText("Test Map", MENUFONT)) / 2, GM.originalH * 5/ 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GM.originalW - MeasureText("Exit", MENUFONT)) / 2, GM.originalH * 7/ 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        if (y < GM.originalH* 3 / 4) {
            if (y < GM.originalH / 4) 
                MM.changeType(campaign);
            else if (y < GM.originalH / 2) 
                MM.changeType(customCampaign);
            else
                MM.changeType(test);
            if (MM.loadmap(GM)) {
                GM.sceneLabel = game;
            }
        }
        else {
            GM.sceneLabel = title;
        }
    }
}

void gameDraw(GameManager& GM, MapManager MM) {
    GM.player.move(GM.WallArr, GM.DamageArr);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (MM.type == test) {
            GM.player.setSpawn(GetMouseX(), GetMouseY());
            GM.player.respawn();
        }
    }
    if (IsKeyPressed(KEY_TAB)) {
        GM.sceneLabel = menu;
    }
    for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
        DrawRectangle(it->x, it->y, it->width, it->height, DAMAGEZONECOLOR);
    }
    for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
        DrawRectangle(it->x, it->y, it->width, it->height, WALLCOLOR);
    }
    DrawRectangle(GM.player.x, GM.player.y, GM.player.width, GM.player.height, PLAYERCOLOR);
}

void titleDraw(GameManager& GM) {
    DrawText("Start the game", (GM.originalW - MeasureText("Start the game", MENUFONT))/2, GM.originalH / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Editor", (GM.originalW - MeasureText("Editor", MENUFONT)) / 2, GM.originalH * 3 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Options", (GM.originalW - MeasureText("Options", MENUFONT)) / 2, GM.originalH * 5 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GM.originalW - MeasureText("Exit", MENUFONT)) / 2, GM.originalH * 7 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        if (y < GM.originalH / 4) 
            GM.sceneLabel = choosing;       
        else if (y < GM.originalH /2) 
            GM.sceneLabel = edit;      
        else if (y < GM.originalH * 3 / 4) 
            GM.sceneLabel = options;      
        else 
            GM.sceneLabel = ext;        
    }
}

void menuDraw(GameManager& GM, MapManager MM) {
    DrawText("Resume the game", (GM.originalW - MeasureText("Resume the game", MENUFONT)) / 2, GM.originalH/4 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit to the title", (GM.originalW - MeasureText("Exit to the title", MENUFONT)) / 2, GM.originalH*3/4 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        if (y > GM.originalH/2) {
            MM.deloadmap(GM);
            GM.sceneLabel = title;
        }
        else {
            GM.sceneLabel = game;
        }
    }
}

void optionsDraw(GameManager& GM) {
    DrawText("Window", (GM.originalW - MeasureText("Window", MENUFONT)) / 2, GM.originalH/10 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Fullscreen", (GM.originalW - MeasureText("Fullscreen", MENUFONT*3/4)) / 4, GM.originalH * 2/ 10 - MENUFONT / 2, MENUFONT * 3 / 4, IsWindowState(FLAG_FULLSCREEN_MODE) ? GREEN : RED);
    DrawText("Windowed", (GM.originalW - MeasureText("Windowed", MENUFONT * 3 / 4)) / 4* 3, GM.originalH * 2/ 10 - MENUFONT / 2, MENUFONT * 3 / 4, IsWindowState(FLAG_FULLSCREEN_MODE) ? RED : GREEN);
    DrawText("Exit", (GM.originalW - MeasureText("Exit", MENUFONT)) / 2, GM.originalH * 7 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int y = GetMouseY();
        int x = GetMouseX();
        if (y > GM.originalH * 5 / 40 && y < GM.originalH * 6 / 20) {
            if (x < GM.originalW / 2) {
                SetWindowState(FLAG_FULLSCREEN_MODE);
            }
            else {
                ClearWindowState(FLAG_FULLSCREEN_MODE);
            }
            SetWindowSize(GM.originalW, GM.originalH);
        }
        else if (y > GM.originalH*3/4){
            GM.sceneLabel = title;
        }
    }
}

void EditorDrawer::editDraw(GameManager& GM, MapManager MM) {
    static short int spawnPointInc = 0;
    static Vector2 spawnPoints[4];
    if (editMode) {
        int x = GetMouseX();
        int y = GetMouseY();
        for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
            DrawRectangle(it->x, it->y, it->width, it->height, DAMAGEZONECOLOR);
        }
        for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
            DrawRectangle(it->x, it->y, it->width, it->height, WALLCOLOR);
        }
        for (int i = 0; i < spawnPointInc; i++) {
            DrawRectangle((int) spawnPoints[i].x, (int) spawnPoints[i].y, 32, 32, PLAYERCOLOR);
        }
        for (int i = 0; i < GM.originalW; i += 20) {
            DrawLine(i, 0, i, GM.originalH, { 0, 0, 0, 100 });
        }
        for (int i = 0; i < GM.originalH; i += 20) {
            DrawLine(0, i, GM.originalW, i, { 0, 0, 0, 100 });
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
        else if (IsKeyPressed(KEY_TWO))
            editMaterial = damageZone;
        else if (IsKeyPressed(KEY_PERIOD))
            drawBlock = false;
        else if (IsKeyPressed(KEY_P)) {
            drawBlock = false;
            makeRespawn = !makeRespawn;
        }
        else if (IsKeyPressed(KEY_D)) {
            for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->x + it->width, (float)it->y + it->height })) {
                    GM.DamageArr.erase(it);
                    return;
                }
            }
            for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->x + it->width, (float)it->y + it->height })) {
                    GM.WallArr.erase(it);
                    return;
                }
            }
            for (int i = 0; i < spawnPointInc; i++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { spawnPoints[i].x, spawnPoints[i].y, 32, 32 })) {
                    for (; i < spawnPointInc - 1; i++) {
                        spawnPoints[i].x = spawnPoints[i + 1].x;
                        spawnPoints[i].y = spawnPoints[i + 1].y;
                    }
                    spawnPointInc--;
                }
            }
        }
        else if (IsKeyPressed(KEY_Q))
            exitView = !exitView;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (drawBlock == false) {
                if (!makeRespawn) {
                    prevX = x;
                    prevY = y;
                    drawBlock = true;
                }
                else if (spawnPointInc < 4){
                    spawnPoints[spawnPointInc].x = (float) x;
                    spawnPoints[spawnPointInc].y = (float) y;
                    spawnPointInc++;
                }
            }
            else if (drawBlock == true) {
                switch (editMaterial) {
                case wall:
                    GM.WallArr.push_back(BackgroundWall(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                case damageZone:
                    GM.DamageArr.push_back(DamageZone(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                }
                drawBlock = false;
            }
        }
    }
    else {
        DrawText("Press e to toggle between helper and edit", GM.originalW/40, GM.originalH/18 - MENUFONT / 2, MENUFONT/2, BLACK);
        DrawText("Press , to bring up a measure of how high player will jump", GM.originalW / 40, GM.originalH * 3 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press . to undo pressing the mouse", GM.originalW / 40, GM.originalH * 5 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press s to save the map and exit", GM.originalW / 40, GM.originalH * 7 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press g to exit without saving the map", GM.originalW / 40, GM.originalH * 9 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press d to delete a selected object", GM.originalW / 40, GM.originalH * 11 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press p to place respawn points", GM.originalW / 40, GM.originalH * 13 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 1 to choose wall", GM.originalW / 40, GM.originalH * 15 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 2 to choose danger zone", GM.originalW / 40, GM.originalH * 17 / 18 - MENUFONT / 2, MENUFONT / 2, BLACK);
    }
    if (IsKeyPressed(KEY_E))
        editMode = !editMode;  
    else if (IsKeyPressed(KEY_G)) {
        GM.sceneLabel = title;
        editMode = false;
        drawBlock = false;
        measure = false;
        editMaterial = wall;
        prevX = prevY = 0;
        MM.deloadmap(GM);
        spawnPointInc = 0;
    }
    else if (IsKeyPressed(KEY_S)) {
        MM.savemap(GM, spawnPoints, spawnPointInc);
        GM.sceneLabel = title;
        editMode = false;
        drawBlock = false;
        measure = false;
        editMaterial = wall;
        prevX = prevY = 0;
        MM.deloadmap(GM);
        spawnPointInc = 0;
    }

}

EditorDrawer::EditorDrawer() {
    editMode = false;
    drawBlock = false;
    measure = false;
    exitView = false;
    makeRespawn = false;
    editMaterial = wall;
    prevX = prevY = 0;
}