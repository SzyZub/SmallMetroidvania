#include "drawing.h"

int main(void)
{
    std::vector <Object*> objList;
    Player* pl = new Player(20, 20);
    Player* pl2 = new Player(511, 222);
    objList.push_back(pl);
    objList.push_back(pl2);
    initScreen();
    while (!WindowShouldClose())  
    {
        mainDraw(objList);
    }
    CloseWindow(); 
    return 0;
}