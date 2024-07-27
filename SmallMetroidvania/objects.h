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
	menu,
	errorLoad
}EnScene;

typedef enum EnLabel {
	player = 0,
	wall,
	damageZone,
	launch,
	items
}EnLabel;

typedef enum ItemLabels {
	none,
	doubleJump
}ItemLabels;

typedef struct SoundLibrary {
	Sound JumpSound;
	Sound DeathSound;
	Sound LaunchSound;
	Sound SelectSound;
}SoundLibrary;

class Object {
public:
	int x, y, width, height, rotation;
	EnLabel label;
};

class Items : public Object {
public:
	ItemLabels itemLabel;
	Items(int lx, int ly, int lwidth, int lheight, int lrotation, ItemLabels llabel);
	Items();
};

class BackgroundWall : public Object {
public:
	BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class DamageZone : public Object {
public:
	DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class LaunchPad : public Object {
public:
	LaunchPad(int lx, int ly, int lwidth, int lheight, int lrotation);
};

class Player : public Object {
private:
	int moveX, moveY, jumped;
	bool isInAir;
public:
	Vector2 spawnPoint;
	int allowedJumps;
	bool respawning;
	double respawnTime;
	Player();
	void setSpawn(int lx, int ly);
	void move(std::vector <BackgroundWall> WallArr,	std::vector <DamageZone> DamageArr,	std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL);
	void respawn();
	void collisionX(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr, std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL);
	void collisionY(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr, std::vector <LaunchPad> LaunchArr, Items& currentItem, SoundLibrary SL);
};

class GameManager {
public:
	int originalW, originalH, framerate;
	EnScene sceneLabel;
public:
	Player player;
	std::vector <BackgroundWall> WallArr;
	std::vector <DamageZone> DamageArr;
	std::vector <LaunchPad> LaunchArr;
	Items currentItem;
	SoundLibrary SL;
	void changeScene(EnScene temp);
	void InitSounds();
	GameManager();
};