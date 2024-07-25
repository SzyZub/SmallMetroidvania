#pragma once
#include "objects.h"
#include "maps.h"

#define BACKGROUNDCOLOR {225, 225, 225, 255}
#define PLAYERCOLOR {122, 122, 200, 255}
#define WALLCOLOR {10, 10, 10, 255}
#define DAMAGEZONECOLOR {200, 50, 50, 255}
#define LAUNCHPADCOLOR1 {200, 200, 50, 255}
#define LAUNCHPADCOLOR2 {200, 150, 50, 255}
#define MENUFONT 64

void initScreen(GameManager GM);
void drawAllObjects(GameManager GM);
void mainDraw(GameManager& GM, MapManager& MM);
void errorLoadDraw(GameManager& GM, MapManager& MM);
void chooseMap(GameManager& GM, MapManager& MM);
void checkBorders(GameManager& Gm, MapManager& MM);
void gameDraw(GameManager& GM, MapManager& MM);
void titleDraw(GameManager& GM);
void menuDraw(GameManager& GM, MapManager MM);
void optionsDraw(GameManager& GM);

class EditorDrawer {
private:
    bool editMode, drawBlock, exitView, measure, makeRespawn;
    EnLabel editMaterial;
    short int prevX, prevY;
public:
    EditorDrawer();
    void editDraw(GameManager& GM, MapManager MM);
};