#include "maps.h"

void deloadmap(std::vector <Object*>& objList) {
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        free(*it);
    }
    objList.clear();
}

void loadmap(int level, std::vector <Object*>& objList) {
    objList.push_back(new BackgroundWall(0, 0, 32, SCREENH));
    objList.push_back(new BackgroundWall(0, 0, SCREENW, 32));
    objList.push_back(new BackgroundWall(0, 736, SCREENW, 32));
    objList.push_back(new BackgroundWall(994, 0, 32, 680));
    objList.push_back(new BackgroundWall(500, 640, 32, 50));
    objList.push_back(new BackgroundWall(400, 640, 32, 200));
    objList.push_back(new Player(40, 704));
}