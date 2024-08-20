#include "drawing.h"

int main(void)
{
    GameManager GameManagerEntity;
    MapManager MapManagerEntity;
    EditorDrawer* EditorDrawerEntity = NULL;
    initScreen(GameManagerEntity);
    GameManagerEntity.initSounds();
    while (!WindowShouldClose() && GameManagerEntity.sceneLabel != ext)    
        mainDraw(GameManagerEntity, MapManagerEntity, &EditorDrawerEntity);
    while (IsSoundPlaying(GameManagerEntity.SoundManagerEntity.SelectSound));
    CloseWindow();
    return 0;
}

//Add 3 different key cards and gates for them
//Add popping in walls(when you click they change the state from on to off and reverse) and item that unlocks them
//Shooting enemies
//Moving platforms/enemies
//Sucking in block
