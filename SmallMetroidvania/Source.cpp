#include "drawing.h"

int main(void)
{
    GameManager GM;
    MapManager MM;
    initScreen(GM);
    GM.InitSounds();
    while (!WindowShouldClose() && GM.sceneLabel != ext)  
    {
        mainDraw(GM, MM);
    }
    CloseWindow(); 
    return 0;
}