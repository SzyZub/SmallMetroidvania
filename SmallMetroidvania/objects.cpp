#include "objects.h"

Player::Player(int sx, int sy, int smX, int smY) {
		 x = sx;
		 y = sy;
		 spawnX = sx;
		 spawnY = sy;
		 moveX = smX;
		 moveY = smY;
		 spawnMoveX = smX;
		 spawnMoveY = smY;
		 width = 32;
		 height = 32;
		 rotation = 0;
		 jumped = false;
		 label = enLabel::player;
}

void Player::move(std::vector <Object*>& objList) {
	collisionX(objList);
	x += moveX;
	collisionY(objList);
	y += moveY;
	moveY += 1;
	if (IsKeyDown(KEY_RIGHT) && moveX < 8)
		moveX += 4;
	else if (IsKeyDown(KEY_LEFT) && moveX > -8)
		moveX -= 4;
	else 
		moveX /= 2;
	if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && !jumped) {
		moveY = -14;
		jumped = true;
	}

}

void Player::die(std::vector <Object*>& objList) {
	x = spawnX;
	y = spawnY;
	moveX = spawnMoveX;
	moveY = spawnMoveY;
}

void Player::collisionX(std::vector <Object*>& objList) {
	for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
		if (moveX == 0)
			return;
		switch((*it)->label) {
			case enLabel::wall:
				while (moveX) {
					if (CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
						if (moveX > 0)
							moveX--;
						else
							moveX++;
					}
					else 
						break;			
				}
				break;
			case enLabel::damageZone:
				if (CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x + moveX, (float)y, (float)width, (float)height })) {
					die(objList);
				}
				break;
		}
	}
}

void Player::collisionY(std::vector <Object*>& objList) {
	for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
		if (moveY == 0)
			return;
		switch ((*it)->label) {
			case enLabel::wall:
				while (moveY) {
					jumped = true;
					if (CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
						if (moveY > 0)
							moveY--;
						else
							moveY++;
						jumped = false;
					}
					else
						break;
				}
			case enLabel::damageZone:
				if (CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
					die(objList);
				}
				break;
		}
	}
}

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = enLabel::wall;
}

DamageZone::DamageZone(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = enLabel::damageZone;
}