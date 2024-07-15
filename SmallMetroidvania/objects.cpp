#include "objects.h"

Object::Object() {
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	label = enLabel::wall;
	rotation = 0;
}

Player::Player(int lx, int ly) {
		 x = lx;
		 y = ly;
		 moveX = 0;
		 moveY = 0;
		 width = 32;
		 height = 32;
		 rotation = 0;
		 jumped = 0;
		 label = enLabel::player;
}

void Player::move(std::vector <Object*> objList) {
	collision(objList);
	x += moveX;
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

void Player::collision(std::vector <Object*> objList) {
	for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
		if (moveX == 0 && moveY == 0)
			return;
		if (enLabel::wall == (*it)->label) {
			while (moveX) {
				if (moveX != 0 && CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x + moveX, (float)y, (float)width, (float)height }))
					if (moveX > 0)
						moveX--;
					else
						moveX++;
				else 
					break;			
			}
			while (moveY) {
				if (moveY != 0 && CheckCollisionRecs({ (float)(*it)->x, (float)(*it)->y, (float)(*it)->width, (float)(*it)->height }, { (float)x, (float)y + moveY, (float)width, (float)height })) {
					if (moveY > 0)
						moveY--;
					else
						moveY++;
					if (moveY >= 0)
						jumped = false;
				}
				else 
					break;
			}
		}
	}
}

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight) {
	x = lx ;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = 0;
	label = enLabel::wall;
}

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight, int lrotation) {
	x = lx;
	y = ly;
	width = lwidth;
	height = lheight;
	rotation = lrotation;
	label = enLabel::wall;
}