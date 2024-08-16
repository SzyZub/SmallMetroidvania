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

//Add 3 different key cards and gates for them
//Add announcer system
//Add popping in walls(when you click they change the state from on to off and reverse) and item that unlocks them
//Shooting enemies
//Moving platforms/enemies
//Sucking in block
//Make every block etc smaller
//make a victory screen
//make a timer that will show how long you have played the game