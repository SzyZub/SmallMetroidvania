#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>
#include <string>

void deloadmap(std::vector <Object*>&);
bool loadmap(int col, int row, std::vector <Object*>&, int mode);
void savemap(std::vector <Object*>& objList, exits exitStruct);