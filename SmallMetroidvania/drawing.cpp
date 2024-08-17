#include "drawing.h"

void initScreen(GameManager GameManagerEntity) {
    InitWindow(GameManagerEntity.originalW, GameManagerEntity.originalH, "MetroidCube");
    SetTargetFPS(GameManagerEntity.framerate);
    InitAudioDevice();
    SetExitKey(KEY_GRAVE);
}

void drawAllObjects(GameManager GameManagerEntity) {
    static Item* temp = NULL;
    for (std::vector <Object>::iterator it = GameManagerEntity.objectArr.begin(); it != GameManagerEntity.objectArr.end(); it++)
        switch (it->label) {
        case water:
            DrawRectangle(it->x, it->y, it->width, it->height, WATERCOLOR);
            break;
        case damageZone:
            DrawRectangle(it->x, it->y, it->width, it->height, DAMAGEZONECOLOR);
            break;
        case launch:
            DrawRectangle(it->x, it->y, it->width, it->height, LAUNCHPADCOLOR1);
            switch (it->rotation) {
            case 0:
                DrawRectangle(it->x + it->width * 3 / 8, it->y + it->height / 2, it->width / 4, it->height / 2, LAUNCHPADCOLOR2);
                break;
            case 90:
                DrawRectangle(it->x, it->y + it->height * 3 / 8, it->width / 2, it->height / 4, LAUNCHPADCOLOR2);
                break;
            case 180:
                DrawRectangle(it->x + it->width * 3 / 8, it->y, it->width / 4, it->height / 2, LAUNCHPADCOLOR2);
                break;
            case 270:
                DrawRectangle(it->x + it->width / 2, it->y + it->height * 3 / 8, it->width / 2, it->height / 4, LAUNCHPADCOLOR2);
                break;
            }
            break;
        case wall:
            DrawRectangle(it->x, it->y, it->width, it->height, WALLCOLOR);
            break;
        case items:
            if (!it->collected) {
                switch (it->itemLabel) {
                case doubleJump:
                    DrawRectangle(it->x, it->y, it->width, it->height, DOUBLEJUMPCOLOR);
                    break;
                case dash:
                    DrawRectangle(it->x, it->y, it->width, it->height, DASHCOLOR);
                    break;
                }
            }
            break;
        }
}

void mainDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity, EditorDrawer** EditorDrawerEntity) {
    BeginDrawing();
    ClearBackground(BACKGROUNDCOLOR);
    switch (GameManagerEntity.sceneLabel) {
    case game: 
        gameDraw(GameManagerEntity, MapManagerEntity);
        break;
    case title:
        titleDraw(GameManagerEntity, EditorDrawerEntity);
        break;
    case options:
        optionsDraw(GameManagerEntity);
        break;
    case choosing:
        chooseMap(GameManagerEntity, MapManagerEntity);
        break;
    case menu:
        menuDraw(GameManagerEntity, MapManagerEntity);
        break;
    case errorLoad:
        errorLoadDraw(GameManagerEntity, MapManagerEntity);
        break;
    case choice:
        yesOrNoDraw(GameManagerEntity);
        break;
    case edit:
        (*EditorDrawerEntity)->editDraw(GameManagerEntity, MapManagerEntity);
        break;
    }
    EndDrawing();
}

void errorLoadDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity) {
    DrawText(TextFormat("Couldn't draw map %i %i", MapManagerEntity.row, MapManagerEntity.col), (GameManagerEntity.originalW - MeasureText(TextFormat("Couldn't draw map %i %i", MapManagerEntity.row, MapManagerEntity.col), 40)) / 2, GameManagerEntity.originalH / 10, 40, BLACK);
    DrawText("Press Tab to go back to menu", (GameManagerEntity.originalW - MeasureText("Press Tab to go back to menu", 40)) / 2, GameManagerEntity.originalH * 3 / 10, 40, BLACK);
    if (IsKeyPressed(KEY_TAB)) {
        MapManagerEntity.deloadmap(GameManagerEntity);
        GameManagerEntity.sceneLabel = title;
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
    }
}

void chooseMap(GameManager& GameManagerEntity, MapManager& MapManagerEntity) {
    DrawText("Main Campaign", (GameManagerEntity.originalW-MeasureText("Main Campaign", MENUFONT))/2, GameManagerEntity.originalH/6-MENUFONT/2, MENUFONT, BLACK);
    DrawText("Test Map", (GameManagerEntity.originalW - MeasureText("Test Map", MENUFONT)) / 2, GameManagerEntity.originalH * 3/ 6 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GameManagerEntity.originalW - MeasureText("Exit", MENUFONT)) / 2, GameManagerEntity.originalH * 5/ 6 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
        MapManagerEntity.row = 1;
        MapManagerEntity.col = 1;
        int y = GetMouseY();
        if (y < GameManagerEntity.originalH* 5 / 6) {
            GameManagerEntity.player.clearItem();
            if (y < GameManagerEntity.originalH / 3) {
                MapManagerEntity.campaginType = campaign;
                MapManagerEntity.loadCampaignState(GameManagerEntity);
            }
            else
                MapManagerEntity.campaginType = test;
            if (MapManagerEntity.loadmap(GameManagerEntity)) {
                GameManagerEntity.sceneLabel = game;
                GameManagerEntity.player.respawn();
            }
        }
        else 
            GameManagerEntity.sceneLabel = title;
    }
}

bool checkBorders(GameManager& GameManagerEntity, MapManager& MapManagerEntity) {
    if (!(GameManagerEntity.player.x > GameManagerEntity.originalW - 8) && !(GameManagerEntity.player.x + 8 < 0) && !(GameManagerEntity.player.y + 8 < 0) && !(GameManagerEntity.player.y > GameManagerEntity.originalH - 8))
        return false;
    int tempAllowedJump = GameManagerEntity.player.allowedJumps;
    bool tempAllowedDash = GameManagerEntity.player.allowedDash;
    MapManagerEntity.deloadmap(GameManagerEntity);
    GameManagerEntity.player.allowedJumps = tempAllowedJump;
    GameManagerEntity.player.allowedDash = tempAllowedDash;
    if (GameManagerEntity.player.x > GameManagerEntity.originalW-16) {
        MapManagerEntity.col++;
        GameManagerEntity.player.x = -16;
    }
    else if (GameManagerEntity.player.x < -GameManagerEntity.player.width) {
        MapManagerEntity.col--;
        GameManagerEntity.player.x = GameManagerEntity.originalW - 16;
    }
    else if (GameManagerEntity.player.y < -GameManagerEntity.player.height) {
        MapManagerEntity.row--;
        GameManagerEntity.player.y = GameManagerEntity.originalH - 16;
    } else if (GameManagerEntity.player.y > GameManagerEntity.originalH - 16) {
        MapManagerEntity.row++;
        GameManagerEntity.player.y = 16;
    }
    if (!MapManagerEntity.loadmap(GameManagerEntity))
        GameManagerEntity.sceneLabel = errorLoad;
    else saveCampaignState(GameManagerEntity, MapManagerEntity);
    return true;
}

void gameDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity) {
    drawAllObjects(GameManagerEntity);
    if (MapManagerEntity.campaginType == test) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            GameManagerEntity.player.spawnPoint.x = (float)GetMouseX();
            GameManagerEntity.player.spawnPoint.y = (float)GetMouseY();
            GameManagerEntity.player.respawn();
            MapManagerEntity.loadmap(GameManagerEntity);
        }
        if (IsKeyPressed(KEY_ONE))
            GameManagerEntity.player.allowedJumps = 2;
        else if (IsKeyPressed(KEY_TWO))
            GameManagerEntity.player.allowedDash = true;
    }
    if (GameManagerEntity.player.isRespawning) {
        DrawText(TextFormat("Respawning in: %i", (int)(GameManagerEntity.player.respawnTime + 4 - GetTime())), (GameManagerEntity.originalW - MeasureText(TextFormat("Respawning in: %d", (int)(GameManagerEntity.player.respawnTime + 4 - GetTime())), 40)) / 2, GameManagerEntity.originalH / 10, 40, BLACK);
        if (GameManagerEntity.player.respawnTime + 3 < GetTime()) {
            GameManagerEntity.player.isRespawning = false;
            GameManagerEntity.player.respawn();
            MapManagerEntity.deloadmap(GameManagerEntity);
            MapManagerEntity.loadCampaignState(GameManagerEntity);
            MapManagerEntity.loadmap(GameManagerEntity);
        }
    }
    else 
        DrawRectangle(GameManagerEntity.player.x, GameManagerEntity.player.y, GameManagerEntity.player.width, GameManagerEntity.player.height, PLAYERCOLOR);
    if (checkBorders(GameManagerEntity, MapManagerEntity)) 
        GameManagerEntity.player.unstuck(GameManagerEntity.objectArr);
    GameManagerEntity.player.move(GameManagerEntity.objectArr, GameManagerEntity.SoundManagerEntity);
    if (IsKeyPressed(KEY_TAB)) 
        GameManagerEntity.sceneLabel = menu;
}

void titleDraw(GameManager& GameManagerEntity, EditorDrawer** EditorDrawerEntity) {
    if ((*EditorDrawerEntity) != NULL) {
        delete (*EditorDrawerEntity);
        (*EditorDrawerEntity) = NULL;
    }
    DrawText("Start the game", (GameManagerEntity.originalW - MeasureText("Start the game", MENUFONT))/2, GameManagerEntity.originalH / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Editor", (GameManagerEntity.originalW - MeasureText("Editor", MENUFONT)) / 2, GameManagerEntity.originalH * 3 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Options", (GameManagerEntity.originalW - MeasureText("Options", MENUFONT)) / 2, GameManagerEntity.originalH * 5 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Exit", (GameManagerEntity.originalW - MeasureText("Exit", MENUFONT)) / 2, GameManagerEntity.originalH * 7 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
        int y = GetMouseY();
        if (y < GameManagerEntity.originalH / 4) 
            GameManagerEntity.sceneLabel = choosing;       
        else if (y < GameManagerEntity.originalH / 2) {
            (*EditorDrawerEntity) = new EditorDrawer();
            GameManagerEntity.sceneLabel = edit;
            GameManagerEntity.player.respawnTime = 0;
        }
        else if (y < GameManagerEntity.originalH * 3 / 4) 
            GameManagerEntity.sceneLabel = options;      
        else 
            GameManagerEntity.sceneLabel = ext;        
    }
}

void menuDraw(GameManager& GameManagerEntity, MapManager MapManagerEntity) {
    GameManagerEntity.player.respawnTime = GetTime();
    DrawText("Resume the game", (GameManagerEntity.originalW - MeasureText("Resume the game", MENUFONT)) / 2, GameManagerEntity.originalH/4 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Save & exit", (GameManagerEntity.originalW - MeasureText("Save & exit", MENUFONT)) / 2, GameManagerEntity.originalH*3/4 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
        int y = GetMouseY();
        if (y > GameManagerEntity.originalH/2) {
            saveCampaignState(GameManagerEntity, MapManagerEntity);
            MapManagerEntity.deloadmap(GameManagerEntity);
            GameManagerEntity.player.isRespawning = false;
            GameManagerEntity.sceneLabel = title;
        }
        else 
            GameManagerEntity.sceneLabel = game;
    }
}

void optionsDraw(GameManager& GameManagerEntity) {
    DrawText("Window", (GameManagerEntity.originalW - MeasureText("Window", MENUFONT)) / 2, GameManagerEntity.originalH/10 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("Fullscreen", (GameManagerEntity.originalW - MeasureText("Fullscreen", MENUFONT*3/4)) / 4, GameManagerEntity.originalH * 2/ 10 - MENUFONT / 2, MENUFONT * 3 / 4, IsWindowState(FLAG_FULLSCREEN_MODE) ? GREEN : RED);
    DrawText("Windowed", (GameManagerEntity.originalW - MeasureText("Windowed", MENUFONT * 3 / 4)) / 4* 3, GameManagerEntity.originalH * 2/ 10 - MENUFONT / 2, MENUFONT * 3 / 4, IsWindowState(FLAG_FULLSCREEN_MODE) ? RED : GREEN);
    DrawText("Sound Volume", (GameManagerEntity.originalW - MeasureText("Sound Volume", MENUFONT)) / 2, GameManagerEntity.originalH * 3 / 10 - MENUFONT / 2, MENUFONT, BLACK);
    for (int i = 0; i <= 10; i++) 
        DrawText(TextFormat("%i", i), (GameManagerEntity.originalW - MeasureText(TextFormat("%i", i), MENUFONT * 3 / 4))*(i + 1)/12, GameManagerEntity.originalH * 2 / 5 - MENUFONT / 2, MENUFONT*3/4, GameManagerEntity.SoundManagerEntity.Volume == i ? GREEN : BLACK);
    DrawText("RESET THE CAMPAIGN PROGRESS", (GameManagerEntity.originalW - MeasureText("RESET THE CAMPAIGN PROGRESS", MENUFONT)) / 2, GameManagerEntity.originalH * 7 / 10 - MENUFONT / 2, MENUFONT, RED);
    DrawText("Exit", (GameManagerEntity.originalW - MeasureText("Exit", MENUFONT)) / 2, GameManagerEntity.originalH * 9 / 10 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
        int y = GetMouseY();
        int x = GetMouseX();
        if (y > GameManagerEntity.originalH * 5 / 40 && y < GameManagerEntity.originalH * 9 / 40) {
            if (x < GameManagerEntity.originalW / 2) 
                SetWindowState(FLAG_FULLSCREEN_MODE);
            else 
                ClearWindowState(FLAG_FULLSCREEN_MODE);
            SetWindowSize(GameManagerEntity.originalW, GameManagerEntity.originalH);
        }
        else if (y > GameManagerEntity.originalH * 7 / 20 && y < GameManagerEntity.originalH * 17/40 && x > GameManagerEntity.originalW * 1 / 24 && x < GameManagerEntity.originalW * 23 / 24) {
            GameManagerEntity.SoundManagerEntity.Volume = (x + GameManagerEntity.originalW * 1 / 24) * 12 / GameManagerEntity.originalW - 1;
            SetMasterVolume((float) GameManagerEntity.SoundManagerEntity.Volume / 10);
        }
        else if (y > GameManagerEntity.originalH * 3 / 5 && y < GameManagerEntity.originalH * 4 / 5) 
            GameManagerEntity.sceneLabel = choice;
        else if (y > GameManagerEntity.originalH*4/5)
            GameManagerEntity.sceneLabel = title;
    }
}

void yesOrNoDraw(GameManager& GameManagerEntity) {
    DrawText("Are you sure?", (GameManagerEntity.originalW - MeasureText("Are you sure?", MENUFONT)) / 2, GameManagerEntity.originalH / 10 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("YES", (GameManagerEntity.originalW - MeasureText("YES", MENUFONT)) / 4, GameManagerEntity.originalH * 3 / 4 - MENUFONT / 2, MENUFONT, BLACK);
    DrawText("NO", (GameManagerEntity.originalW - MeasureText("NO", MENUFONT)) * 3/ 4, GameManagerEntity.originalH * 3 / 4 - MENUFONT / 2, MENUFONT, BLACK);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        PlaySound(GameManagerEntity.SoundManagerEntity.SelectSound);
        int y = GetMouseY();
        int x = GetMouseX();
        if (y > GameManagerEntity.originalH * 11 / 16 && y < GameManagerEntity.originalH * 13 / 16) {
            if (x > (GameManagerEntity.originalW - MeasureText("YES", MENUFONT)) / 4 - MeasureText("YES", MENUFONT) / 3 && x < (GameManagerEntity.originalW - MeasureText("YES", MENUFONT)) / 4 + MeasureText("YES", MENUFONT) * 4 / 3) {
                std::remove("Maps/Campaign/CampaignSave.txt");
                GameManagerEntity.sceneLabel = options;
            }
            else if (x > (GameManagerEntity.originalW - MeasureText("NO", MENUFONT)) * 3 / 4 - MeasureText("NO", MENUFONT) / 3 && x < (GameManagerEntity.originalW - MeasureText("NO", MENUFONT)) * 3 / 4 + MeasureText("NO", MENUFONT) * 4 / 3) 
                GameManagerEntity.sceneLabel = options;       
        }
    }
}

void EditorDrawer::editDraw(GameManager& GameManagerEntity, MapManager MapManagerEntity) {
    if (GameManagerEntity.player.respawnTime + 2 > GetTime() && GetTime() > 2) 
        DrawText("You didn't place any respawn points", (GameManagerEntity.originalW - MeasureText("You didn't place any respawn points", MENUFONT)) / 2, GameManagerEntity.originalH * 1 / 8 - MENUFONT / 2, MENUFONT, BLACK);
    if (isEditMode) {
        int x = GetMouseX();
        int y = GetMouseY();
        drawAllObjects(GameManagerEntity);
        for (int i = 0; i < spawnPointInc; i++) 
            DrawRectangle((int) spawnPoints[i].x, (int) spawnPoints[i].y, 32, 32, PLAYERCOLOR);
        for (int i = 0; i < GameManagerEntity.originalW; i += 20) 
            DrawLine(i, 0, i, GameManagerEntity.originalH, { 0, 0, 0, 100 });
        for (int i = 0; i < GameManagerEntity.originalH; i += 20) 
            DrawLine(0, i, GameManagerEntity.originalW, i, { 0, 0, 0, 100 });
        if (isMeasure) {
            DrawLineEx({ (float)x, (float)y }, { (float)x, (float)y - 110 }, 4, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x, (float)y - 230 }, 2, { 120, 120, 120, 255 });
            DrawLineEx({ (float)x, (float)y }, { (float)x + 220, (float)y }, 4, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x - 220, (float)y }, 4, BLACK);
            DrawLineEx({ (float)x, (float)y }, { (float)x + 420, (float)y }, 3, {50, 50, 50, 255});
            DrawLineEx({ (float)x, (float)y }, { (float)x - 420, (float)y }, 3, { 50, 50, 50, 255 });
            DrawLineEx({ (float)x, (float)y }, { (float)x + 620, (float)y }, 2, { 120, 120, 120, 255 });
            DrawLineEx({ (float)x, (float)y }, { (float)x - 620, (float)y }, 2, { 120, 120, 120, 255 });
        }
        if (isDrawBlock) 
            DrawCircle(prevX, prevY, 5, RED);
        if (IsKeyPressed(KEY_COMMA))
            isMeasure = !isMeasure;
        else if (IsKeyPressed(KEY_ZERO)) {
            editMaterial = player;
            isDrawBlock = false;
        }
        else if (IsKeyPressed(KEY_ONE)) {
            editMaterial = items;
            isDrawBlock = false;
        }
        else if (IsKeyPressed(KEY_TWO)) 
            editMaterial = wall;
        else if (IsKeyPressed(KEY_THREE)) 
            editMaterial = damageZone;
        else if (IsKeyPressed(KEY_FOUR)) 
            editMaterial = launch;
        else if (IsKeyPressed(KEY_FIVE))
            editMaterial = water;
        else if (IsKeyPressed(KEY_PERIOD))
            isDrawBlock = false;
        else if (IsKeyPressed(KEY_D)) {
            for (std::vector <Object>::iterator it = GameManagerEntity.objectArr.begin(); it != GameManagerEntity.objectArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    GameManagerEntity.objectArr.erase(it);
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
            isExitView = !isExitView;
        else if (IsKeyPressed(KEY_R)) {
            for (std::vector <Object>::iterator it = GameManagerEntity.objectArr.begin(); it != GameManagerEntity.objectArr.end(); it++) {
                if (CheckCollisionPointRec({ (float)x, (float)y }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
                    switch (it->label) {
                    case launch:
                        it->rotation += 90;
                        it->rotation %= 360;
                        break;
                    case items:
                        Item* temp = (Item*)&(*it);
                        temp->itemLabel = (ItemLabel)((temp->itemLabel) % 2 + 1);
                        break;
                    }
                    return;
                }
            }      
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (isDrawBlock == false) {
                if (editMaterial != items && editMaterial != player) {
                    prevX = x;
                    prevY = y;
                    isDrawBlock = true;
                }
                else {
                    for (std::vector <Object>::iterator it = GameManagerEntity.objectArr.begin(); it != GameManagerEntity.objectArr.end(); it++) 
                        if (CheckCollisionRecs({ (float)x + 1, (float)y + 1, (float)GameManagerEntity.player.width - 1, (float)GameManagerEntity.player.height - 1 }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height }))
                            return;
                    if (editMaterial == player && spawnPointInc < 4) {
                        spawnPoints[spawnPointInc].x = (float)x;
                        spawnPoints[spawnPointInc].y = (float)y;
                        spawnPointInc++;
                    }
                    else if (editMaterial == items) 
                        GameManagerEntity.objectArr.push_back(Item(x, y, 32, 32, 0, doubleJump, false));                   
                }
            }
            else if (isDrawBlock == true) {
                switch (editMaterial) {
                case wall:
                    GameManagerEntity.objectArr.push_back(Wall(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                case damageZone:
                    GameManagerEntity.objectArr.push_back(DamageZone(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                case launch:
                    GameManagerEntity.objectArr.push_back(LaunchPad(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                case water:
                    GameManagerEntity.objectArr.push_back(Water(prevX > x ? x - 2 : prevX - 2, prevY > y ? y - 2 : prevY - 2, abs(x - prevX) + 4, abs(y - prevY) + 4, 0));
                    break;
                }
                isDrawBlock = false;
            }
        }
    }
    else {
        DrawText("Press tab to toggle between helper and edit", GameManagerEntity.originalW/40, GameManagerEntity.originalH/26, MENUFONT/2, BLACK);
        DrawText("Press , to bring up a measure of how high player will jump", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 3 / 26, MENUFONT / 2, BLACK);
        DrawText("Press . to undo pressing the mouse", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 5 / 26, MENUFONT / 2, BLACK);
        DrawText("Press s to save the map and exit", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 7 / 26, MENUFONT / 2, BLACK);
        DrawText("Press g to exit without saving the map", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 9 / 26, MENUFONT / 2, BLACK);
        DrawText("Press d to delete a selected object", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 11 / 26, MENUFONT / 2, BLACK);
        DrawText("Press r to rotate eligible objects or cycle through items", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 13 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 0 to place respawn points", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 15 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 1 to place a item", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 17 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 2 to choose wall", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 19 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 3 to choose danger zone", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 21 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 4 to choose launch pad", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 23 / 26, MENUFONT / 2, BLACK);
        DrawText("Press 5 to choose water", GameManagerEntity.originalW / 40, GameManagerEntity.originalH * 25 / 26, MENUFONT / 2, BLACK);
    }
    if (IsKeyPressed(KEY_TAB))
        isEditMode = !isEditMode;  
    else if (IsKeyPressed(KEY_G)) {
        GameManagerEntity.sceneLabel = title;
        isEditMode = false;
        isDrawBlock = false;
        isMeasure = false;
        editMaterial = wall;
        prevX = prevY = 0;
        MapManagerEntity.deloadmap(GameManagerEntity);
        spawnPointInc = 0;
    }
    else if (IsKeyPressed(KEY_S)) { 
        if (spawnPointInc < 1) 
            GameManagerEntity.player.respawnTime = GetTime();
        else {
            MapManagerEntity.savemap(GameManagerEntity, spawnPoints, spawnPointInc);
            GameManagerEntity.sceneLabel = title;
            isEditMode = false;
            isDrawBlock = false;
            isMeasure = false;
            editMaterial = wall;
            prevX = prevY = 0;
            MapManagerEntity.deloadmap(GameManagerEntity);
            spawnPointInc = 0;
        }
    }

}

EditorDrawer::EditorDrawer() {
    isEditMode = false;
    isDrawBlock = false;
    isMeasure = false;
    isExitView = false;
    editMaterial = wall;
    spawnPointInc = 0;
    prevX = prevY = 0;
    for (int i = 0; i < 4; i++) {
        spawnPoints->x = 0;
        spawnPoints->y = 0;
    }
}