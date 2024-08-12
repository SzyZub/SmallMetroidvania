#pragma once
#include "objects.h"
#include "maps.h"

#define BACKGROUNDCOLOR {225, 225, 225, 255}
#define PLAYERCOLOR {122, 122, 200, 255}
#define WALLCOLOR {10, 10, 10, 255}
#define DAMAGEZONECOLOR {200, 50, 50, 255}
#define LAUNCHPADCOLOR1 {200, 200, 50, 255}
#define LAUNCHPADCOLOR2 {200, 150, 50, 255}
#define DOUBLEJUMPCOLOR {0, 0, 255, 255}
#define MENUFONT 64

void initScreen(GameManager GameManagerEntity);
void drawAllObjects(GameManager GameManagerEntity);
void mainDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity);
void errorLoadDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity);
void chooseMap(GameManager& GameManagerEntity, MapManager& MapManagerEntity);
bool checkBorders(GameManager& GameManagerEntity, MapManager& MapManagerEntity);
void unstuck(GameManager& GameManagerEntity);
void gameDraw(GameManager& GameManagerEntity, MapManager& MapManagerEntity);
void titleDraw(GameManager& GameManagerEntity);
void menuDraw(GameManager& GameManagerEntity, MapManager MapManagerEntity);
void optionsDraw(GameManager& GameManagerEntity);

class EditorDrawer {
private:
    bool isEditMode, isDrawBlock, isExitView, isMeasure;
    EntityLabel editMaterial;
    Vector2 spawnPoints[4];
    short int prevX, prevY, spawnPointInc;
public:
    EditorDrawer();
    void editDraw(GameManager& GameManagerEntity, MapManager MapManagerEntity);
};