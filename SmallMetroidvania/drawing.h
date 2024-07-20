#pragma once
#include "objects.h"
#include "maps.h"

#define BACKGROUNDCOLOR {225, 225, 225, 255}
#define PLAYERCOLOR {122, 122, 200, 255}
#define WALLCOLOR {0, 0, 0, 255}
#define DAMAGEZONECOLOR {200, 50, 50, 255}
#define MENUFONT 64

void initScreen(GameManager GM);
void mainDraw(GameManager& GM, MapManager& MM);
void chooseMap(GameManager& GM, MapManager& MM);
void gameDraw(GameManager& GM, MapManager MM);
void titleDraw(GameManager& GM);
void menuDraw(GameManager& GM, MapManager MM);
void optionsDraw(GameManager& GM);

class EditorDrawer {
private:
    bool editMode, drawBlock, exitView, measure;
    EnLabel editMaterial;
    short int prevX, prevY;
public:
    EditorDrawer();
    void editDraw(GameManager& GM, MapManager MM);
};