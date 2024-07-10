#include "drawing.h"
#include "maps.h"

int main(void)
{
    std::vector <Object*> objList;
    loadmap(1, objList);
    initScreen();
    while (!WindowShouldClose())  
    {
        mainDraw(objList);
        for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
            if (enLabel::player == (*it)->label) {
                (*it)->move(objList);
            }
        }
    }
    CloseWindow(); 
    return 0;
}