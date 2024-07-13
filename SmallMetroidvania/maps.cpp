#include "maps.h"

void deloadmap(std::vector <Object*>& objList) {
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        free(*it);
    }
    objList.clear();
}

void loadmap(int col, int row, std::vector <Object*>& objList) {
    std::string mapData;
    int temp;
    int val1, val2, val3, val4;
    std::ifstream readFile("Maps/Campaign/1.txt");
    while (std::getline(readFile, mapData)) {
        std::size_t pos = mapData.find(' ');
        std::size_t prevPos = pos + 1;
        temp = std::stoi(mapData.substr(0, pos));
        switch (temp) {
        case 0:
            pos = mapData.find(' ', prevPos);
            val1 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val2 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val3 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val4 = std::stoi(mapData.substr(prevPos, pos));
            objList.push_back(new BackgroundWall(val1, val2, val3, val4));
            break;
        case 1:
            pos = mapData.find(' ', prevPos);
            val1 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val2 = std::stoi(mapData.substr(prevPos, pos));
            objList.push_back(new Player(val1, val2));
            break;
        }
    }
    readFile.close();
}
