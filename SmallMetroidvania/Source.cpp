#include "drawing.h"

int main(void)
{
    GameManager GameManagerEntity;
    MapManager MapManagerEntity;
    initScreen(GameManagerEntity);
    GameManagerEntity.initSounds();
    while (!WindowShouldClose() && GameManagerEntity.sceneLabel != ext)    
        mainDraw(GameManagerEntity, MapManagerEntity);
    CloseWindow(); 
    return 0;
}

// TODO:: 
//LOAD CAMPAIGN STATE WHEN ENTERING ONE
//ADD VOLUME SLIDER
//REMOVE SOUND WHEN EXITING THE GAME
//FIX BEING STUCK IN WALL WHEN TRANSITIONING BETWEEN MAPS