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

//Shooting enemies
