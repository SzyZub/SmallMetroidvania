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
	wall,
	damageZone
}enLabel;

class Object {
public:
	int x, y, width, height, rotation;
	enLabel label;
	virtual void move(std::vector <Object*>& objList) {};
	virtual void die(std::vector <Object*>& objList) {};
};

class Player : public Object {
private:
	int moveX, moveY, spawnX, spawnY, spawnMoveX, spawnMoveY;
	bool jumped;
public:
	Player(int sx, int sy, int smX, int smY);
	void move(std::vector <Object*>& objList);
	void die(std::vector <Object*>& objList);
	void collisionX(std::vector <Object*>& objList);
	void collisionY(std::vector <Object*>& objList);
};

class BackgroundWall : public Object {
public:
	BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class DamageZone : public Object {
public:
	DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation);
};