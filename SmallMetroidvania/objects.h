#pragma once
#include "raylib.h"
#include <vector>

#define SCREENW 1024
#define SCREENH 768

typedef struct exits {
	bool left, right, down, up;
}exits;

typedef enum enLabel {
	player = 0,
	wall
}enLabel;

class Object {
public:
	Object();
	int x, y, width, height, rotation;
	enLabel label;
	virtual void move(std::vector <Object*> objList) {};
};

class Player : public Object {
private:
	int moveX, moveY;
	bool jumped;
public:
	Player(int lx, int ly);
	void move(std::vector <Object*> objList);
	void collisionX(std::vector <Object*> objList);
	void collisionY(std::vector <Object*> objList);
};

class BackgroundWall : public Object {
public:
	BackgroundWall(int lx, int ly, int lwidth, int lheight);
	BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation);
};