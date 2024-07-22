#include "objects.h"

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = wall;
}

DamageZone::DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = damageZone;
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
	jumped = false;
	label = player;
	respawning = false;
	respawnTime = 0;
}

void Player::setSpawn(int lx, int ly) {
	spawnPoint.x = (float) lx;
	spawnPoint.y = (float) ly;
}

void Player::move(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr) {
	if (respawning){
		if (respawnTime + 3 < GetTime()) {
			respawning = false;
			respawn();
		}
	}
	else {
		collisionX(WallArr, DamageArr);
		x += moveX;
		collisionY(WallArr, DamageArr);
		y += moveY;
		moveY += 1;
		if (IsKeyDown(KEY_RIGHT) && moveX < 8)
			moveX += 4;
		else if (IsKeyDown(KEY_LEFT) && moveX > -8)
			moveX -= 4;
		else
			moveX /= 2;
		if (IsKeyDown(KEY_R)) {
			respawning = true;
			respawnTime = GetTime();
		}
		if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && !jumped) {
			moveY = -14;
			jumped = true;
		}
	}

}

void Player::respawn() {
	x = spawnPoint.x;
	y = spawnPoint.y;
	moveX = 0;
	moveY = 0;
	jumped = false;
	rotation = 0;
}

void Player::collisionX(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr) {
	if (moveX != 0) {
		for (std::vector <DamageZone>::iterator it = DamageArr.begin(); it != DamageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height }))
				respawning = true;
				respawnTime = GetTime();
		}
		for (std::vector <BackgroundWall>::iterator it = WallArr.begin(); it != WallArr.end(); it++) {
			while (moveX) {
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
					if (moveX > 0)
						moveX--;
					else if (moveX < 0)
						moveX++;
					else {
						return;
					}
				}
				else
					break;
			}
		}
	}
}

void Player::collisionY(std::vector <BackgroundWall> WallArr, std::vector <DamageZone> DamageArr) {
	if (moveY != 0) {
		for (std::vector <DamageZone>::iterator it = DamageArr.begin(); it != DamageArr.end(); it++) {
			if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height }))
				respawning = true;
				respawnTime = GetTime();
		}
		for (std::vector <BackgroundWall>::iterator it = WallArr.begin(); it != WallArr.end(); it++) {
			while (moveY) {
				jumped = true;
				if (CheckCollisionRecs({ (float)it->x, (float)it->y, (float)it->width, (float)it->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
					jumped = false;
					if (moveY > 0)
						moveY--;
					else if (moveY < 0)
						moveY++;
					else {
						return;
					}
				}
				else
					break;
			}
		}
	}
}

GameManager::GameManager() {
	originalH = 720;
	originalW = 1280;
	framerate = 60;
	sceneLabel = title;
}

void GameManager::changeScene(EnScene temp) {
	sceneLabel = temp;
}