#include "objects.h"

Item::Item(int readX, int readY, int readWith, int readHeight, int readRotation, ItemLabel readItemLabel, bool readCollected) {
	x = readX;
	y = readY;
	width = PLAYERSIZE;
	height = PLAYERSIZE;
	rotation = 0;
	itemLabel = readItemLabel;
	label = items;
	collected = readCollected;
}

Item::Item() {
	label = items;
	itemLabel = none;
	collected = false;
}

Wall::Wall(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = 0;
	label = wall;
}

DamageZone::DamageZone(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = 0;
	label = damageZone;
}

LaunchPad::LaunchPad(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = readRotation;
	label = launch;
}

Water::Water(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = 0;
	label = water;
}

Player::Player() {
	spawnPoint = { -50, -50 };
	x = -50;
	y = -50;
	moveX = 0;
	moveY = 0;
	width = PLAYERSIZE;
	height = PLAYERSIZE;
	rotation = 0;
	jumped = 0;
	allowedJumps = 1;
	label = player;
	isInAir = false;
	isRespawning = false;
	isDashed = false;
	allowedDash = false;
	respawnTime = 0;
}

void Player::move(std::vector <Object>& objectArr, SoundLibrary SoundManagerEntity) {
	if(!isRespawning) {
		bool waterCol = collision(objectArr, SoundManagerEntity);		
		x += moveX;
		y += moveY;
		friction();
		if (IsKeyDown(KEY_LEFT_SHIFT) && allowedDash && !isDashed) {
			if (moveX > 0) 
				moveX = PLAYERMAXDASH;
			else if (moveX < 0)
				moveX = -PLAYERMAXDASH;
			moveY = 0;
			isDashed = true;
		}
		if (IsKeyDown(KEY_RIGHT) && moveX < PLAYERMAXX)
			moveX += PLAYERACC;
		else if (IsKeyDown(KEY_LEFT) && moveX > -PLAYERMAXX)
			moveX -= PLAYERACC;
		if (waterCol) {
			isDashed = false;
			isInAir = false;
			if (moveY < PLAYERMAXY/2)
				moveY += GRAVITY;
			if (moveY > 0) 
				jumped = allowedJumps - 1;			
		} else if (!isDashed || abs(moveX) < PLAYERMAXDASH / 2)
			moveY += GRAVITY;
		if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && jumped < allowedJumps && abs(moveX) < PLAYERMAXDASH / 2 && ((isInAir && allowedJumps != 1) || (!isInAir))) {
			isInAir == true ? jumped = allowedJumps : jumped++;
			moveY = -PLAYERMAXY;
			if (waterCol)
				PlaySound(SoundManagerEntity.JumpWaterSound);
			else
				PlaySound(SoundManagerEntity.JumpSound);
		}
		if (IsKeyDown(KEY_R)) {
			isRespawning = true;
			respawnTime = GetTime();
			PlaySound(SoundManagerEntity.DeathSound);
		}
	}

}

void Player::respawn() {
	x = (int) spawnPoint.x;
	y = (int) spawnPoint.y;
	moveX = 0;
	moveY = 0;
}

bool Player::collision(std::vector <Object>& objectArr, SoundLibrary SoundManagerEntity) {
	isInAir = true;
	bool returnVal = false;
	int clear = 0;
	Item* temp = NULL;
	if (moveX != 0 || moveY != 0) {
		for (std::vector <Object>::iterator it = objectArr.begin(); it != objectArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y + moveY, (float)width, (float)height })) {
				switch (it->label) {
					case damageZone:
						isRespawning = true;
						respawnTime = GetTime();
						PlaySound(SoundManagerEntity.DeathSound);
						return false;
						break;
					case launch:
						if (it->rotation == 90) {
							moveX = -PLAYERMAXX * 6;
							moveY = 0;
						}
						else if (it->rotation == 180) {
							moveY = -PLAYERMAXX * 5;
							moveX = 0;
						}
						else if (it->rotation == 270) {
							moveX = PLAYERMAXX * 6;
							moveY = 0;
						}
						PlaySound(SoundManagerEntity.LaunchSound);
						break;
					case wall:
						clear = 0;
						while (moveX != 0 || moveY != 0) {
							clear = 0;
							if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
								if (y < it->y)
									moveY--;
								else
									moveY++;
								if (y < it->y && moveY == 0) {
									jumped = 0;
									isInAir = false;
									isDashed = false;
								}
							}
							else
								clear++;
							if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
								if (x < it->x)
									moveX--;
								else
									moveX++;
							}
							else
								clear++;
							if (clear == 2)
								break;
						}
						break;
					case water:
						returnVal = true;
						break;
					case items:
						if (it->itemLabel == doubleJump)
							allowedJumps = 2;
						else if (it->itemLabel == dash)
							allowedDash = true;
						it->itemLabel = none;
						it->collected = true;
						break;
				}
			}
		}
	}
	return returnVal;
}

void Player::friction() {
	if (moveX > 0)
		moveX -= GRAVITY;
	else if (moveX < 0)
		moveX += GRAVITY;
	if (isDashed) {
		if (moveX > 0)
			moveX -= GRAVITY;
		else
			moveX += GRAVITY;
	}
}

void Player::clearItem() {
	allowedJumps = 1;
	allowedDash = false;
}

void Player::unstuck(std::vector <Object> objectArr) {
	for (std::vector <Object>::iterator it = objectArr.begin(); it != objectArr.end(); it++) {
		if (it->label == wall) {
			while (CheckCollisionRecs({ (float)x, (float)y, (float)width, (float)height }, { (float)it->x, (float)it->y, (float)it->width, (float)it->height })) {
				if (x + width / 2 < it->x + it->width / 2)
					x--;
				else
					x++;
				if (y + height / 2 < it->y + it->height / 2)
					y--;
				else
					y++;
			}
		}
	}
}

GameManager::GameManager() {
	originalH = 720;
	originalW = 1280;
	framerate = 60;
	sceneLabel = title;
	totalTime = 0;
	isAnnounce = true;
	SoundManagerEntity = { NULL };
}

void GameManager::initSounds() {
	SoundManagerEntity.JumpSound = LoadSound("Sounds/jump.wav");
	SoundManagerEntity.JumpWaterSound = LoadSound("Sounds/jumpWater.wav");
	SoundManagerEntity.DeathSound = LoadSound("Sounds/death.wav");
	SoundManagerEntity.LaunchSound = LoadSound("Sounds/launch.wav");
	SoundManagerEntity.SelectSound = LoadSound("Sounds/select.wav");
	SoundManagerEntity.Volume = 10;
}
