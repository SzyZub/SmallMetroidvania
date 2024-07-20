#pragma once
#include "raylib.h"
#include <vector>

typedef enum EnScene {
	game = 0,
	title,
	edit,
	options,
	ext,
	choosing,
	menu
}EnScene;

typedef enum EnLabel {
	player = 0,
	wall,
	damageZone
}EnLabel;

class Object {
public:
	int x, y, width, height, rotation;
	EnLabel label;
};

class BackgroundWall : public Object {
public:
	BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class DamageZone : public Object {
public:
	DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class Player : public Object {
private:
	int moveX, moveY;
	int spawnX, spawnY;
	bool jumped;
public:
	Player();
	void setSpawn(int lx, int ly);
	void move(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr);
	void respawn();
	void collisionX(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr);
	void collisionY(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr);
};

class GameManager {
public:
	int originalW, originalH, framerate;
	EnScene sceneLabel;
public:
	Player player;
	std::vector <BackgroundWall> WallArr;
	std::vector <DamageZone> DamageArr;
	void changeScene(EnScene temp);
	GameManager();
};