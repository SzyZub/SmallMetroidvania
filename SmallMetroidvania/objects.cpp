#include "objects.h"

Item::Item(int readX, int readY, int readWith, int readHeight, int readRotation, ItemLabel readItemLabel) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = readRotation;
	itemLabel = readItemLabel;
	label = items;
}

Item::Item() {
	label = items;
	itemLabel = none;
}

Wall::Wall(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = readRotation;
	label = wall;
}

DamageZone::DamageZone(int readX, int readY, int readWith, int readHeight, int readRotation) {
	x = readX;
	y = readY;
	width = readWith;
	height = readHeight;
	rotation = readRotation;
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
	rotation = readRotation;
	label = water;
}

Player::Player() {
	spawnPoint = { -50, -50 };
	x = -50;
	y = -50;
	moveX = 0;
	moveY = 0;
	width = 32;
	height = 32;
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

void Player::move(std::vector <Wall> wallArr, std::vector <DamageZone> damageArr, std::vector <LaunchPad> launchArr, Item& currentItem, SoundLibrary SoundManagerEntity, std::vector <Water> waterArr) {
	if(!isRespawning) {
		collisionX(wallArr, damageArr, launchArr, currentItem, SoundManagerEntity);
		x += moveX;
		collisionY(wallArr, damageArr, launchArr, currentItem, SoundManagerEntity);
		y += moveY;
		if (IsKeyDown(KEY_LEFT_SHIFT) && allowedDash && !isDashed) {
			if (moveX > 0) 
				moveX = 30;
			else if (moveX < 0)
				moveX = -30;
			moveY = 0;
			isDashed = true;
		}
		else if (IsKeyDown(KEY_RIGHT) && moveX < 6)
			moveX += 3;
		else if (IsKeyDown(KEY_LEFT) && moveX > -6)
			moveX -= 3;
		else if (moveX > 0)
			moveX --;
		else if (moveX < 0)
			moveX ++;
		if (isDashed) {
			if (moveX > 0)
				moveX--;
			else
				moveX++;
		}
		if (checkColWater(waterArr)) {
			isDashed = false;
			isInAir = false;
			if (moveY < 3)
				moveY++;
			if (moveY > 0) 
				jumped = allowedJumps - 1;			
		} else if (!isDashed || abs(moveX) < 10)
			moveY++;
		if (IsKeyDown(KEY_R)) {			
			isRespawning = true;
			respawnTime = GetTime();
			PlaySound(SoundManagerEntity.DeathSound);
		}
		if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && jumped < allowedJumps && abs(moveX) < 10) {
			if (isInAir && allowedJumps != 1) {
				jumped = 2;
				moveY = -15;
				if (checkColWater(waterArr))
					PlaySound(SoundManagerEntity.JumpWaterSound);
				else
					PlaySound(SoundManagerEntity.JumpSound);
			}
			else if (!isInAir) {
				jumped++;
				moveY = -15;
				if (checkColWater(waterArr))
					PlaySound(SoundManagerEntity.JumpWaterSound);
				else
					PlaySound(SoundManagerEntity.JumpSound);
			}
		}
	}

}

void Player::respawn() {
	x = (int) spawnPoint.x;
	y = (int) spawnPoint.y;
	moveX = 0;
	moveY = 0;
	jumped = allowedJumps;
}

void Player::collisionX(std::vector <Wall> wallArr, std::vector <DamageZone> damageArr, std::vector <LaunchPad> launchArr, Item& currentItem, SoundLibrary SoundManagerEntity) {
	if (moveX != 0) {
		if (CheckCollisionRecs({ (float)x, (float)y, (float) width, (float) height }, { (float)currentItem.x, (float)currentItem.y, (float) currentItem.width, (float)currentItem.height })) {
			if (currentItem.itemLabel == doubleJump)
				allowedJumps = 2;
			else if (currentItem.itemLabel == dash)
				allowedDash = true;
			currentItem.itemLabel = none;
		}
		for (std::vector <LaunchPad>::iterator it = launchArr.begin(); it != launchArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveX, (float)width, (float)height })) {
				if (it->rotation == 90)
					moveX = -30;
				else if (it->rotation == 270)
					moveX = 30;
				PlaySound(SoundManagerEntity.LaunchSound);
			}
		}
		for (std::vector <DamageZone>::iterator it = damageArr.begin(); it != damageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
				isRespawning = true;
				respawnTime = GetTime();
				PlaySound(SoundManagerEntity.DeathSound);
			}
		}
		for (std::vector <Wall>::iterator it = wallArr.begin(); it != wallArr.end(); it++) {
			while (moveX) {
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
					if (moveX > 0)
						moveX--;
					else if (moveX < 0)
						moveX++;
					else 
						return;				
				}
				else
					break;
			}
		}
	}
}

void Player::collisionY(std::vector <Wall> wallArr, std::vector <DamageZone> damageArr, std::vector <LaunchPad> launchArr, Item& currentItem, SoundLibrary SoundManagerEntity) {
	isInAir = true;;
	if (moveY != 0) {
		for (std::vector <LaunchPad>::iterator it = launchArr.begin(); it != launchArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
				if (it->rotation == 180)
					moveY = -25;
				else if (it->rotation == 90)
					moveX = -30;
				else if (it->rotation == 270)
					moveX = 30;
				PlaySound(SoundManagerEntity.LaunchSound);
			}
		}
		for (std::vector <DamageZone>::iterator it = damageArr.begin(); it != damageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
				isRespawning = true;
				respawnTime = GetTime();
				PlaySound(SoundManagerEntity.DeathSound);
			}
		}
		for (std::vector <Wall>::iterator it = wallArr.begin(); it != wallArr.end(); it++) {
			while (moveY) {
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
					jumped = 0;
					isInAir = false;
					isDashed = false;
					if (moveY > 0)
						moveY--;
					else if (moveY < 0)
						moveY++;
					else 
						return;
				}
				else
					break;
			}
		}
	}
}

bool Player::checkColWater(std::vector <Water> waterArr) {
	for (std::vector <Water>::iterator it = waterArr.begin(); it != waterArr.end(); it++) {
		if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
			return true;
		}
	}
	return false;
}

void Player::clearItem() {
	allowedJumps = 1;
	allowedDash = false;
}

GameManager::GameManager() {
	originalH = 720;
	originalW = 1280;
	framerate = 60;
	sceneLabel = title;
	currentItem = Item();
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
