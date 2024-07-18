#pragma once
#include "objects.h"
#include "maps.h"

#define BACKGROUNDCOLOR {225, 225, 225, 255}
#define PLAYERCOLOR {122, 122, 200, 255}
#define WALLCOLOR {0, 0, 0, 255}

typedef enum EnScene {
    game = 0,
    title,
    edit,
    options,
    ext,
    choosing,
    menu
}EnScene;

void initScreen();
void chooseMap(std::vector <Object*>& objList, EnScene& programScene);
void mainDraw(std::vector <Object*>& objList, EnScene &programScene);
void gameDraw(std::vector <Object*>& objList, EnScene &programScene);
void titleDraw(std::vector <Object*>& objList, EnScene &programScene);
void menuDraw(std::vector <Object*>& objList, EnScene& programScene);
void optionsDraw(EnScene& programScene);

class Drawer {
private:
    bool editMode, drawBlock, exitView, measure;
    enLabel editMaterial;
    short int prevX, prevY;
    exits exitStruct;
public:
    Drawer();
    void editDraw(std::vector <Object*>& objList, EnScene& programScene);
};