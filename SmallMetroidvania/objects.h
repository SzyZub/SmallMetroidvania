#pragma once
#include "raylib.h"
#include <vector>

typedef enum SceneLabel {
	game = 0,
	title,
	edit,
	options,
	ext,
	choosing,
	menu,
	errorLoad,
	choice
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
};

class Item : public Object {
public:
	ItemLabel itemLabel;
	Item(int readX, int readY, int readWith, int readHeight, int readRotation, ItemLabel readItemLabel);
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
	void move(std::vector <Object> objectArr, SoundLibrary SoundManagerEntity, std::vector <Water> waterArr);
	void respawn();
	bool collision(std::vector <Object> objectArr, SoundLibrary SoundManagerEntity);
	void clearItem();
	void unstuck(std::vector <Object> objectArr);
	void friction();
};

class GameManager {
public:
	GameManager();
	int originalW, originalH, framerate;
	SceneLabel sceneLabel;
	SoundLibrary SoundManagerEntity;
	Player player;
	std::vector <Object> objectArr;
	std::vector <Water> waterArr;
	void initSounds();
};
