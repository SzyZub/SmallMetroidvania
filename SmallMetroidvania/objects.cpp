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
		 label = enLabel::player;
}

void Player::move(std::vector <Object*> objList) {
	if (IsKeyDown(KEY_RIGHT) && moveX < 8)
		moveX += 2;
	if (IsKeyDown(KEY_LEFT) && moveX > -8)
		moveX -= 2;
	if (IsKeyPressed(KEY_UP) && moveY == 0)
		moveY = -8;
	x += moveX;
	y += moveY;
	moveX /= 2;
	moveY += 2;
}

BackgroundWall::BackgroundWall(int lx, int ly, int lwidth, int lheight) {
	x = lx;
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