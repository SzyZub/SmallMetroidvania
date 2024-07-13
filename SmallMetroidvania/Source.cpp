#include "drawing.h"

int main(void)
{
    std::vector <Object*> objList;
    EnScene programScene = title;
    initScreen();
    while (!WindowShouldClose() && programScene != ext)  
    {
        mainDraw(objList, programScene);
    }
    CloseWindow(); 
    return 0;
}