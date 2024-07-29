#include "drawing.h"

int main(void)
{
    GameManager GameManagerEntity;
    MapManager MapManagerEntity;
    initScreen(GameManagerEntity);
    GameManagerEntity.initSounds();
    while (!WindowShouldClose() && GameManagerEntity.sceneLabel != ext)    
        mainDraw(GameManagerEntity, MapManagerEntity);
    while (IsSoundPlaying(GameManagerEntity.SoundManagerEntity.SelectSound));
    CloseWindow();
    return 0;
}

// TODO:: 
//FIX BEING STUCK IN WALL WHEN TRANSITIONING BETWEEN MAPS