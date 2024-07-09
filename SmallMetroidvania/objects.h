#pragma once
#include "raylib.h"

typedef enum class enLabel {
	player = 0,
	wall
}enLabel;

class Object {
public:
	int x, y, width, height, rotation;
	enLabel label;
	static bool checkCol(Object a, Object b);
};

class Player : public Object {
public:
	Player(int lx, int ly);
};

class BackgroundWall : public Object {
public:

};