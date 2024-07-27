#include "drawing.h"

void initScreen(GameManager GM) {
    InitWindow(GM.originalW, GM.originalH, "MetroidCube");
    SetTargetFPS(GM.framerate);
    InitAudioDevice();
    SetExitKey(KEY_GRAVE);
}

void drawAllObjects(GameManager GM) {
    for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
        DrawRectangle(it->x, it->y, it->width, it->height, DAMAGEZONECOLOR);
    }
    for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
        DrawRectangle(it->x, it->y, it->width, it->height, WALLCOLOR);
    }
    for (std::vector <LaunchPad>::iterator it = GM.LaunchArr.begin(); it != GM.LaunchArr.end(); it++) {
        DrawRectangle(it->x, it->y, it->width, it->height, LAUNCHPADCOLOR1);
        switch (it->rotation) {
        case 0:
            DrawRectangle(it->x + it->width * 3/8, it->y + it->height/2, it->width/4, it->height/2, LAUNCHPADCOLOR2);
            break;
        case 90:
            DrawRectangle(it->x, it->y + it->height * 3 / 8, it->width / 2, it->height / 4, LAUNCHPADCOLOR2);
            break;
        case 180:
            DrawRectangle(it->x + it->width * 3 / 8, it->y, it->width / 4, it->height / 2, LAUNCHPADCOLOR2);
            break;
        case 270:
            DrawRectangle(it->x + it->width / 2, it->y + it->height *3 / 8, it->width / 2, it->height / 4, LAUNCHPADCOLOR2);
            break;
        }
    }
    switch (GM.currentItem.itemLabel) {
    case doubleJump:
        DrawRectangle(GM.currentItem.x, GM.currentItem.y, GM.currentItem.width, GM.currentItem.height, DOUBLEJUMPCOLOR);
        break;
    }
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
    case errorLoad:
        errorLoadDraw(GM, MM);
        break;
    case edit:
        static EditorDrawer draw;
        draw.editDraw(GM, MM);
        break;
    }
    EndDrawing();
}

void errorLoadDraw(GameManager& GM, MapManager& MM) {
    DrawText(TextFormat("Couldn't draw map %i %i", MM.row, MM.col), (GM.originalW - MeasureText(TextFormat("Couldn't draw map %i %i", MM.row, MM.col), 40)) / 2, GM.originalH / 10, 40, BLACK);
    DrawText("Press Tab to go back to menu", (GM.originalW - MeasureText("Press Tab to go back to menu", 40)) / 2, GM.originalH * 3 / 10, 40, BLACK);
    if (IsKeyPressed(KEY_TAB)) {
        MM.deloadmap(GM);
        GM.sceneLabel = title;
        PlaySound(GM.SL.SelectSound);
    }
}

void chooseMap(GameManager& GM, MapManager& MM) {
    DrawText("Main Campaign", (GM.originalW-MeasureText("Main Campaign", MENUFONT))/2, GM.originalH/8-MENUFONT/2, MENUFONT, BLACK);
    DrawText("Custom Campaign", (GM.originalW - MeasureText("Custom Campaign", MENUFONT)) / 2, GM.originalH * 3/8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Test Map", (GM.originalW - MeasureText("Test Map", MENUFONT)) / 2, GM.originalH * 5/ 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GM.originalW - MeasureText("Exit", MENUFONT)) / 2, GM.originalH * 7/ 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GM.SL.SelectSound);
        MM.row = 1;
        MM.col = 1;
        int y = GetMouseY();
        if (y < GM.originalH* 3 / 4) {
            if (y < GM.originalH / 4) {
                MM.changeType(campaign);
            }
            else if (y < GM.originalH / 2) 
                MM.changeType(customCampaign);
            else
                MM.changeType(test);
            if (MM.loadmap(GM)) {
                GM.sceneLabel = game;
                GM.player.respawn();
            }
        }
        else {
            GM.sceneLabel = title;
        }
    }
}

void checkBorders(GameManager& GM, MapManager& MM) {
    if (GM.player.x > GM.originalW - 8) {
        MM.deloadmap(GM);
        MM.col++;
        GM.player.x = 16;
        if(!MM.loadmap(GM))
            GM.changeScene(errorLoad);
    }
    else if (GM.player.x + 8 < 0) {
        MM.deloadmap(GM);
        MM.col--;
        GM.player.x = GM.originalW - 16;
        if (!MM.loadmap(GM))
            GM.changeScene(errorLoad);
    }
    else if (GM.player.y + 8 < 0) {
        MM.deloadmap(GM);
        MM.row--;
        GM.player.y = GM.originalH - 16;;
        if (!MM.loadmap(GM))
            GM.changeScene(errorLoad);
    } else if (GM.player.y > GM.originalH - 8) {
        MM.deloadmap(GM);
        MM.row++;
        GM.player.y = 16;
        if (!MM.loadmap(GM))
            GM.changeScene(errorLoad);
    }
}

void gameDraw(GameManager& GM, MapManager& MM) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (MM.type == test) {
            GM.player.setSpawn(GetMouseX(), GetMouseY());
            GM.player.respawn();
        }
    }
    if (GM.player.respawning) {
        DrawText(TextFormat("Respawning in: %i", (int)(GM.player.respawnTime + 4 - GetTime())), (GM.originalW - MeasureText(TextFormat("Respawning in: %d", (int)(GM.player.respawnTime + 4 - GetTime())), 40)) / 2, GM.originalH / 10, 40, BLACK);
        if (GM.player.respawnTime + 3 < GetTime()) {
            GM.player.respawning = false;
            GM.player.respawn();
        }
    }
    else {
        DrawRectangle(GM.player.x, GM.player.y, GM.player.width, GM.player.height, PLAYERCOLOR);
    }
    GM.player.move(GM.WallArr, GM.DamageArr, GM.LaunchArr, GM.currentItem ,GM.SL);
    drawAllObjects(GM);
    checkBorders(GM, MM);
    if (IsKeyPressed(KEY_TAB)) {
        GM.sceneLabel = menu;
    }
}

void titleDraw(GameManager& GM) {
    DrawText("Start the game", (GM.originalW - MeasureText("Start the game", MENUFONT))/2, GM.originalH / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Editor", (GM.originalW - MeasureText("Editor", MENUFONT)) / 2, GM.originalH * 3 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Options", (GM.originalW - MeasureText("Options", MENUFONT)) / 2, GM.originalH * 5 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GM.originalW - MeasureText("Exit", MENUFONT)) / 2, GM.originalH * 7 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GM.SL.SelectSound);
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
    GM.player.respawnTime = GetTime();
    DrawText("Resume the game", (GM.originalW - MeasureText("Resume the game", MENUFONT)) / 2, GM.originalH/4 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit to the title", (GM.originalW - MeasureText("Exit to the title", MENUFONT)) / 2, GM.originalH*3/4 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GM.SL.SelectSound);
        int y = GetMouseY();
        if (y > GM.originalH/2) {
            MM.deloadmap(GM);
            GM.player.respawning = false;
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
        PlaySound(GM.SL.SelectSound);
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
    if (GM.player.respawnTime + 2 > GetTime() && GetTime() > 2) {
        DrawText("You didn't place any respawn points", (GM.originalW - MeasureText("You didn't place any respawn points", MENUFONT)) / 2, GM.originalH * 1 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    }
    if (editMode) {
        int x = GetMouseX();
        int y = GetMouseY();
        drawAllObjects(GM);
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
            DrawLineEx({ (float)x, (float)y }, { (float)x, (float)y - 120 }, 3, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x + 210, (float)y }, 3, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x - 210, (float)y }, 3, BLACK);
        }
        if (drawBlock) {
            DrawCircle(prevX, prevY, 5, RED);
        }
        if (IsKeyPressed(KEY_COMMA))
            measure = !measure;
        else if (IsKeyPressed(KEY_ZERO)) {
            editMaterial = player;
            drawBlock = false;
        }
        else if (IsKeyPressed(KEY_ONE)) {
            editMaterial = items;
            drawBlock = false;
        }
        else if (IsKeyPressed(KEY_TWO)) {
            editMaterial = wall;
        }
        else if (IsKeyPressed(KEY_THREE)) {
            editMaterial = damageZone;
        }
        else if (IsKeyPressed(KEY_FOUR)) {
            editMaterial = launch;
        }
        else if (IsKeyPressed(KEY_PERIOD))
            drawBlock = false;
        else if (IsKeyPressed(KEY_D)) {
            for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    GM.DamageArr.erase(it);
                    return;
                }
            }
            for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    GM.WallArr.erase(it);
                    return;
                }
            }
            for (std::vector <LaunchPad>::iterator it = GM.LaunchArr.begin(); it != GM.LaunchArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    GM.LaunchArr.erase(it);
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
            if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)GM.currentItem.x, (float)GM.currentItem.y, 32, 32 })) {
                GM.currentItem.itemLabel = none;
            }
        }
        else if (IsKeyPressed(KEY_Q))
            exitView = !exitView;
        else if (IsKeyPressed(KEY_R)) {
            for (std::vector <LaunchPad>::iterator it = GM.LaunchArr.begin(); it != GM.LaunchArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    it->rotation += 90;
                    it->rotation %= 360;
                    return;
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (drawBlock == false) {
                if (editMaterial != items && editMaterial != player) {
                    prevX = x;
                    prevY = y;
                    drawBlock = true;
                }
                else {
                    for (std::vector <DamageZone>::iterator it = GM.DamageArr.begin(); it != GM.DamageArr.end(); it++) {
                        if (CheckCollisionRecs({ (float)x, (float)y, 32, 32 }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                            return;
                        }
                    }
                    for (std::vector <BackgroundWall>::iterator it = GM.WallArr.begin(); it != GM.WallArr.end(); it++) {
                        if (CheckCollisionRecs({ (float)x + 1, (float)y + 1, 31, 31 }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                            return;
                        }
                    }
                    for (std::vector <LaunchPad>::iterator it = GM.LaunchArr.begin(); it != GM.LaunchArr.end(); it++) {
                        if (CheckCollisionRecs({ (float)x + 1, (float)y + 1, 31, 31 }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                            return;
                        }
                    }
                    if (editMaterial == player && spawnPointInc < 4) {
                        spawnPoints[spawnPointInc].x = (float)x;
                        spawnPoints[spawnPointInc].y = (float)y;
                        spawnPointInc++;
                    }
                    else if (editMaterial == items) {
                        GM.currentItem = Items(x, y, 32, 32, 0, doubleJump);
                    }
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
                case launch:
                    GM.LaunchArr.push_back(LaunchPad(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                }
                drawBlock = false;
            }
        }
    }
    else {
        DrawText("Press e to toggle between helper and edit", GM.originalW/40, GM.originalH/24 - MENUFONT / 2, MENUFONT/2, BLACK);
        DrawText("Press , to bring up a measure of how high player will jump", GM.originalW / 40, GM.originalH * 3 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press . to undo pressing the mouse", GM.originalW / 40, GM.originalH * 5 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press s to save the map and exit", GM.originalW / 40, GM.originalH * 7 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press g to exit without saving the map", GM.originalW / 40, GM.originalH * 9 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press d to delete a selected object", GM.originalW / 40, GM.originalH * 11 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press r to rotate eligible objects or cycle through items", GM.originalW / 40, GM.originalH * 13 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 0 to place respawn points", GM.originalW / 40, GM.originalH * 15 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 1 to place a item", GM.originalW / 40, GM.originalH * 17 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 2 to choose wall", GM.originalW / 40, GM.originalH * 19 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 3 to choose danger zone", GM.originalW / 40, GM.originalH * 21 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
        DrawText("Press 4 to choose launch pad", GM.originalW / 40, GM.originalH * 23 / 24 - MENUFONT / 2, MENUFONT / 2, BLACK);
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
        if (spawnPointInc < 1) {
            GM.player.respawnTime = GetTime();
        }
        else {
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

}

EditorDrawer::EditorDrawer() {
    editMode = false;
    drawBlock = false;
    measure = false;
    exitView = false;
    editMaterial = wall;
    spawnPointInc = 0;
    prevX = prevY = 0;
}