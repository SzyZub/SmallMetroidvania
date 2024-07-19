#include "maps.h"

void deloadmap(std::vector <Object*>& objList) {
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        delete* it;
    }
    objList.clear();
}

bool loadmap(int col, int row, std::vector <Object*>& objList, int mode) {
    std::string mapData;
    int temp;
    int val1, val2, val3, val4, val5;
    std::ifstream readFile;
    if (mode == 0) {
        readFile.open("Maps/Campaign/11.txt");
    }
    else if (mode == 1) {
        readFile.open("Maps/Custom/11.txt");
    }
    else {
        readFile.open("Maps/Test/Custom.txt");
    }
    if (!readFile.is_open()) {
        return false;
    }
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
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val5 = std::stoi(mapData.substr(prevPos, pos));
            objList.push_back(new BackgroundWall(val1, val2, val3, val4, val5));
            break;
        case 1:
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
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val5 = std::stoi(mapData.substr(prevPos, pos));
            objList.push_back(new DamageZone(val1, val2, val3, val4, val5));
            break;
        }
    }
    readFile.close();
    return true;
}

void savemap(std::vector <Object*>& objList, exits exitStruct) {
    std::ofstream writeFile("Maps/Test/Custom.txt");
    for (std::vector <Object*>::iterator it = objList.begin(); it != objList.end(); it++) {
        switch ((*it)->label) {
        case wall:
            writeFile << 0 << ' ' << (*it)->x << ' ' << (*it)->y << ' ' << (*it)->width << ' ' << (*it)->height << ' ' << (*it)->rotation << '\n';
            break;
        case damageZone:
            writeFile << 1 << ' ' << (*it)->x << ' ' << (*it)->y << ' ' << (*it)->width << ' ' << (*it)->height << ' ' << (*it)->rotation << '\n';
        }
    }
    writeFile << 99 << ' ' << exitStruct.up << ' ' << exitStruct.down << ' ' << exitStruct.left << ' ' << exitStruct.right << '\n';
    writeFile.close();
}