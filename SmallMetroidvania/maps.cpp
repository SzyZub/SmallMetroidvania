#include "maps.h"

MapManager::MapManager() {
    row = 1;
    col = 1;
    campaginType = campaign;
}

void MapManager::deloadmap(GameManager& GameManagerEntity) {
    GameManagerEntity.wallArr.clear();
    GameManagerEntity.damageArr.clear();
    GameManagerEntity.launchArr.clear();
    GameManagerEntity.currentItem.itemLabel = none;
}

bool MapManager::loadmap(GameManager& GameManagerEntity) {
    std::string mapData;
    int spawnPointInc = 0;
    Vector2 spawnPoints[4];
    int tempLabel;
    int val1, val2, val3, val4, val5;
    std::ifstream readFile;
    if (campaginType == campaign) {
        std::string mapName = "Maps/Campaign/" + std::to_string(row) + std::to_string(col) + ".txt";
        readFile.open(mapName);
    }
    else if (campaginType == customCampaign) {
        std::string mapName = "Maps/Custom/" + std::to_string(row) + std::to_string(col) + ".txt";
        readFile.open(mapName);
    }
    else
        readFile.open("Maps/Test/Custom.txt");
    if (!readFile.is_open())
        return false;
    while (std::getline(readFile, mapData)) {
        std::size_t pos = mapData.find(' ');
        std::size_t prevPos = pos + 1;
        tempLabel = std::stoi(mapData.substr(0, pos));
        switch (tempLabel) {
        case 0:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.wallArr.push_back(Wall(val1, val2, val3, val4, val5));
            break;
        case 1:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.damageArr.push_back(DamageZone(val1, val2, val3, val4, val5));
            break;
        case 2:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.launchArr.push_back(LaunchPad(val1, val2, val3, val4, val5));
            break;
        case 99: 
            pos = mapData.find(' ', prevPos);
            val1 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val2 = std::stoi(mapData.substr(prevPos, pos));
            spawnPoints[spawnPointInc].x = (float) val1;
            spawnPoints[spawnPointInc].y = (float) val2;
            spawnPointInc++;
            break;
        case 3:
            if (GameManagerEntity.player.allowedJumps != 2) {
                fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
                GameManagerEntity.currentItem = Item(val1, val2, val3, val4, val5, doubleJump);
            }
            break;
        }
    }
    int bestSpawn = 0;
    for (int i = 1; i < spawnPointInc; i++) 
        if (sqrt(spawnPoints[bestSpawn].x * spawnPoints[bestSpawn].x + spawnPoints[bestSpawn].y * spawnPoints[bestSpawn].y) < sqrt(spawnPoints[i].x * spawnPoints[i].x + spawnPoints[i].y * spawnPoints[i].y)) 
            bestSpawn = i;
    GameManagerEntity.player.spawnPoint.x = (float)spawnPoints->x;
    GameManagerEntity.player.spawnPoint.y = (float)spawnPoints->y;
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

void MapManager::savemap(GameManager GameManagerEntity, Vector2 spawnPoints[4], int spawnPointsNum) {
    std::ofstream writeFile("Maps/Test/Custom.txt");
    for (std::vector <Wall>::iterator it = GameManagerEntity.wallArr.begin(); it != GameManagerEntity.wallArr.end(); it++) 
        writeFile << 0 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    for (std::vector <DamageZone>::iterator it = GameManagerEntity.damageArr.begin(); it != GameManagerEntity.damageArr.end(); it++) 
        writeFile << 1 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    for (std::vector <LaunchPad>::iterator it = GameManagerEntity.launchArr.begin(); it != GameManagerEntity.launchArr.end(); it++) 
        writeFile << 2 << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    if (GameManagerEntity.currentItem.itemLabel != none) {
        switch (GameManagerEntity.currentItem.itemLabel) {
        case doubleJump:
            writeFile << 3 << ' ';
            break;
        }
        writeFile << GameManagerEntity.currentItem.x << ' ' << GameManagerEntity.currentItem.y << ' ' << GameManagerEntity.currentItem.width << ' ' << GameManagerEntity.currentItem.height << ' ' << GameManagerEntity.currentItem.rotation << '\n';
    }
    for (int i = 0; i < spawnPointsNum; i++) 
        writeFile << 99 << ' ' << spawnPoints[i].x << ' ' << spawnPoints[i].y << ' ' << '\n';
    writeFile.close();
}

void saveCampaignSate(GameManager GameManagerEntity, MapManager MapManagerEntity) {
    if (MapManagerEntity.campaginType == test) 
        return;
    std::ofstream writeFile;
    if (MapManagerEntity.campaginType == campaign)
        writeFile.open("Maps/Campaign/CampaignSave.txt");
    else 
        writeFile.open("Maps/CustomCampaign/CampaignSave.txt");
    writeFile << GameManagerEntity.player.spawnPoint.x << ' ' << GameManagerEntity.player.spawnPoint.y << '\n';
    writeFile << MapManagerEntity.row << ' ' << MapManagerEntity.col << '\n';
    writeFile << GameManagerEntity.player.allowedJumps << '\n';
    writeFile.close();
}