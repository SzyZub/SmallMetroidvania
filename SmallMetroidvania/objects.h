#pragma once
#include "raylib.h"
#include <string>
#include <vector>
 
constexpr int PLAYERSIZE = 24;
constexpr int PLAYERMAXX = 5;
constexpr int PLAYERMAXY = 12;
constexpr int PLAYERMAXDASH = 20;
constexpr int GRAVITY = 1;
constexpr int PLAYERACC = 2;

typedef enum SceneLabel {
	game = 0,
	title,
	edit,
	options,
	ext,
	choosing,
	menu,
	errorLoad,
	choice,
	victory
}SceneLabel;

typedef enum EntityLabel {
	player = 0,
	wall,
	damageZone,
	launch,
	items,
	water
}EntityLabel;

typedef enum ItemLabel {
	none,
	doubleJump,
	dash
}ItemLabel;

typedef struct SoundLibrary {
	Sound JumpSound;
	Sound JumpWaterSound;
	Sound DeathSound;
	Sound LaunchSound;
	Sound SelectSound;
	int Volume;
}SoundLibrary;

class Object {
public:
	int x, y, width, height, rotation;
	EntityLabel label;
	ItemLabel itemLabel;
	bool collected;
};

class Item : public Object {
public:
	Item(int readX, int readY, int readWith, int readHeight, int readRotation, ItemLabel readItemLabel, bool readConnected);
	Item();
};

class Wall : public Object {
public:
	Wall(int readX, int readY, int readWith, int readHeight, int readRotation);
};

class DamageZone : public Object {
public:
	DamageZone(int readX, int readY, int readWith, int readHeight, int readRotation);
};

class LaunchPad : public Object {
public:
	LaunchPad(int readX, int readY, int readWith, int readHeight, int readRotation);
};

class Water : public Object {
public:
	Water(int readX, int readY, int readWith, int readHeight, int readRotation);
};

class Player : public Object {
public:
	Player();
	int moveX, moveY, jumped, allowedJumps;
	bool isInAir, isRespawning, allowedDash, isDashed;
	double respawnTime;
	Vector2 spawnPoint;
	void move(std::vector <Object>& objectArr, SoundLibrary SoundManagerEntity);
	void respawn();
	bool collision(std::vector <Object>& objectArr, SoundLibrary SoundManagerEntity);
	void clearItem();
	void unstuck(std::vector <Object> objectArr);
	void friction();
};

class GameManager {
public:
	GameManager();
	int originalW, originalH, framerate;
	long long totalTime;
	bool isAnnounce;
	std::string announceText;
	SceneLabel sceneLabel;
	SoundLibrary SoundManagerEntity;
	Player player;
	std::vector <Object> objectArr;
	void initSounds();
};
