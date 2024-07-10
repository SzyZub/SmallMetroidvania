#pragma once
#include "objects.h"

#define BACKGROUNDCOLOR {200, 200, 200, 255}
#define PLAYERCOLOR {122, 122, 200, 255}
#define WALLCOLOR {0, 0, 0, 255}

void initScreen();
void mainDraw(std::vector <Object*> objList);
