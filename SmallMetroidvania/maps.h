#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>
#include <string>

void deloadmap(std::vector <Object*>&);
void loadmap(int col, int row, std::vector <Object*>&);
void savemap(std::vector <Object*>& objList, exits exitStruct);