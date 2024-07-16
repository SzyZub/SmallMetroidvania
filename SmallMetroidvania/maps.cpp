#include "maps.h"

void deloadmap(std::vector <Object*>& objList) {
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        delete* it;
    }
    objList.clear();
}

void loadmap(int col, int row, std::vector <Object*>& objList) {
    std::string mapData;
    int temp;
    int val1, val2, val3, val4;
    std::ifstream readFile("Maps/Campaign/11.txt");
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

void savemap(std::vector <Object*>& objList, exits exitStruct) {
    std::ofstream writeFile("Maps/Test/Custom.txt");
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        switch ((*it)->label) {
        case wall:
            writeFile << 0 << ' ' << (*it)->x << ' ' << (*it)->y << ' ' << (*it)->width << ' ' << (*it)->height << '\n';
        }
    }
    writeFile << 99 << ' ' << exitStruct.up << ' ' << exitStruct.down << ' ' << exitStruct.left << ' ' << exitStruct.right << '\n';
    writeFile.close();
}