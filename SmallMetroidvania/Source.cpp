#include "drawing.h"

int main(void)
{
    std::vector <Object*> objList;
    initScreen();
    while (!WindowShouldClose())  
    {
        mainDraw(objList);
    }
    CloseWindow(); 
    return 0;
}