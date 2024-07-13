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
    ext
}EnScene;

void initScreen();
void mainDraw(std::vector <Object*>& objList, EnScene &programScene);
void gameDraw(std::vector <Object*>& objList, EnScene &programScene);
void titleDraw(std::vector <Object*>& objList, EnScene &programScene);