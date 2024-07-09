#pragma once
#include "objects.h"
#include <vector>

#define SCREENW 1024
#define SCREENH 768
#define BACKGROUNDCOLOR {200, 200, 200, 255}

void initScreen();
void mainDraw(std::vector <Object*> objList);
