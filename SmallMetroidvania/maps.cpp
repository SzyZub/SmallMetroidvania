#include "maps.h"

MapManager::MapManager() {
    row = 1;
    col = 1;
    type = campaign;
}

void MapManager::changeType(gameType temp) {
    type = temp;
}

void MapManager::deloadmap(GameManager& temp) {
    temp.WallArr.clear();
    temp.DamageArr.clear();
    temp.LaunchArr.clear();
    temp.currentItem.itemLabel = none;
    temp.player.allowedJumps = 1;
}

bool MapManager::loadmap(GameManager& temp) {
    std::string mapData;
    int spawnPointInc = 0;
    Vector2 spawnPoints[4];
    int tempLabel;
    int val1, val2, val3, val4, val5;
    std::ifstream readFile;
    if (type == campaign) {
        std::string mapName = "Maps/Campaign/" + std::to_string(row) + std::to_string(col) + ".txt";
        readFile.open(mapName);
    }
    else if (type == customCampaign) {
        std::string mapName = "Maps/Custom/" + std::to_string(row) + std::to_string(col) + ".txt";
        readFile.open(mapName);
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
        tempLabel = std::stoi(mapData.substr(0, pos));
        switch (tempLabel) {
        case 0:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            temp.WallArr.push_back(BackgroundWall(val1, val2, val3, val4, val5));
            break;
        case 1:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            temp.DamageArr.push_back(DamageZone(val1, val2, val3, val4, val5));
            break;
        case 2:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            temp.LaunchArr.push_back(LaunchPad(val1, val2, val3, val4, val5));
            break;
        case 99: 
            pos = mapData.find(' ', prevPos);
            val1 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val2 = std::stoi(mapData.substr(prevPos, pos));
            spawnPoints[spawnPointInc].x = val1;
            spawnPoints[spawnPointInc].y = val2;
            spawnPointInc++;
            break;
        case 3:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            temp.currentItem = Items(val1, val2, val3, val4, val5, doubleJump);
            break;
        }
    }
    int bestSpawn = 0;
    for (int i = 1; i < spawnPointInc; i++) {
        if (sqrt(spawnPoints[bestSpawn].x * spawnPoints[bestSpawn].x + spawnPoints[bestSpawn].y * spawnPoints[bestSpawn].y) < sqrt(spawnPoints[i].x * spawnPoints[i].x + spawnPoints[i].y * spawnPoints[i].y)) {
            bestSpawn = i;
        }
    }
    temp.player.setSpawn(spawnPoints->x, spawnPoints->y);
    readFile.close();
    return true;
}

void fillValues(int& val1, int& val2, int& val3, int& val4, int& val5, std::size_t& pos, std::size_t& prevPos, std::string& mapData) {
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
}

void MapManager::savemap(GameManager temp, Vector2 spawnPoints[4], int spawnPointsNum) {
    std::ofstream writeFile("Maps/Test/Custom.txt");
    for (std::vector <BackgroundWall>::iterator it = temp.WallArr.begin(); it != temp.WallArr.end(); it++) {
        writeFile << 0 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    }
    for (std::vector <DamageZone>::iterator it = temp.DamageArr.begin(); it != temp.DamageArr.end(); it++) {
        writeFile << 1 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    }
    for (std::vector <LaunchPad>::iterator it = temp.LaunchArr.begin(); it != temp.LaunchArr.end(); it++) {
        writeFile << 2 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    }
    if (temp.currentItem.itemLabel != none) {
        switch (temp.currentItem.itemLabel) {
        case doubleJump:
            writeFile << 3 << ' ';
            break;
        }
        writeFile << temp.currentItem.x << ' ' << temp.currentItem.y << ' ' << temp.currentItem.width << ' ' << temp.currentItem.height << ' ' << temp.currentItem.rotation << '\n';
    }
    for (int i = 0; i < spawnPointsNum; i++) {
        writeFile << 99 << ' ' << spawnPoints[i].x << ' ' << spawnPoints[i].y << ' ' << '\n';
    }
    writeFile.close();
}