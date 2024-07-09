#include "objects.h"

bool Object::checkCol(Object a, Object b) {
		return CheckCollisionRecs({ (float)a.x, (float)a.y, (float)a.width, (float)b.width }, { (float)b.x, (float)b.y, (float)b.width, (float)b.height });
}

Player::Player(int lx, int ly) {
		 x = lx;
		 y = ly;
		 width = 55;
		 height = 55;
		 label = enLabel::player;
}
