#include "maps.h"

MapManager::MapManager() {
    row = 1;
    col = 1;
    campaginType = campaign;
}

void MapManager::deloadmap(GameManager& GameManagerEntity) {
    GameManagerEntity.objectArr.clear();
    GameManagerEntity.player.clearItem();
}

bool MapManager::loadmap(GameManager& GameManagerEntity) {
    GameManagerEntity.isAnnounce = false;
    GameManagerEntity.announceText.clear();
    std::string mapData;
    int spawnPointInc = 0;
    Vector2 spawnPoints[4] = { 0, 0 };
    int val1, val2, val3, val4, val5;
    ItemLabel tempItemLabel = none;
    bool isItemCollected = false;
    std::ifstream readFile;
    if (campaginType == campaign) 
        readFile.open("Maps/Campaign/" + std::to_string(row) + std::to_string(col) + ".txt");
    else
        readFile.open("Maps/Test/Custom.txt");
    if (!readFile.is_open())
        return false;
    while (std::getline(readFile, mapData)) {
        std::size_t pos = mapData.find(' ');
        std::size_t prevPos = pos + 1;
        switch (std::stoi(mapData.substr(0, pos))) {
        case 0:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            break;
        case 10:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            GameManagerEntity.objectArr.back().wallColor = red;
            break;
        case 11:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            GameManagerEntity.objectArr.back().wallColor = blue;
            break;
        case 12:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            GameManagerEntity.objectArr.back().wallColor = green;
            break;
        case 13:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            GameManagerEntity.objectArr.back().wallColor = popOff;
            break;
        case 14:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Wall(val1, val2, val3, val4, val5));
            GameManagerEntity.objectArr.back().wallColor = popOn;
            break;
        case 1:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(DamageZone(val1, val2, val3, val4, val5));
            break;
        case 2:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(LaunchPad(val1, val2, val3, val4, val5));
            break;
        case 3:
            tempItemLabel = doubleJump;
            if (GameManagerEntity.player.allowedJumps == 2)
                isItemCollected = true;
        case 4: 
            if (tempItemLabel == none) {
                tempItemLabel = dash;
                if (GameManagerEntity.player.allowedDash == true)
                    isItemCollected = true;
            }
        case 6:
            if (tempItemLabel == none) 
                tempItemLabel = trophy;
        case 7:
            if (tempItemLabel == none) {
                tempItemLabel = redKey;
                if (GameManagerEntity.player.hasRed == true)
                    isItemCollected = true;
            }
        case 8:
            if (tempItemLabel == none) {
                tempItemLabel = blueKey;
                if (GameManagerEntity.player.hasBlue == true)
                    isItemCollected = true;
            }
        case 9:
            if (tempItemLabel == none) {
                tempItemLabel = greenKey;
                if (GameManagerEntity.player.hasGreen == true)
                    isItemCollected = true;
            }
        case 15:
            if (tempItemLabel == none) {
                tempItemLabel = popControl;
                if (GameManagerEntity.player.hasPopControl == true)
                    isItemCollected = true;
            }
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Item(val1, val2, val3, val4, val5, tempItemLabel, isItemCollected));
            tempItemLabel = none;
            isItemCollected = false;
            break;
        case 5:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(Water(val1, val2, val3, val4, val5));
            break;
        case 16:
            fillValues(val1, val2, val3, val4, val5, pos, prevPos, mapData);
            GameManagerEntity.objectArr.push_back(GravWall(val1, val2, val3, val4, val5));
            break;
        case 98:
            GameManagerEntity.announceText = mapData.substr(pos+1, mapData.length());
            GameManagerEntity.player.respawnTime = GetTime();
            GameManagerEntity.isAnnounce = true;
            break;
        case 99:
            pos = mapData.find(' ', prevPos);
            val1 = std::stoi(mapData.substr(prevPos, pos));
            prevPos = pos + 1;
            pos = mapData.find(' ', prevPos);
            val2 = std::stoi(mapData.substr(prevPos, pos));
            spawnPoints[spawnPointInc].x = (float)val1;
            spawnPoints[spawnPointInc].y = (float)val2;
            spawnPointInc++;
            break;
        }
    }
    readFile.close();
    int bestSpawn = 0;
    for (int i = 1; i < spawnPointInc; i++) 
        if (sqrt(spawnPoints[bestSpawn].x * spawnPoints[bestSpawn].x + spawnPoints[bestSpawn].y * spawnPoints[bestSpawn].y) < sqrt(spawnPoints[i].x * spawnPoints[i].x + spawnPoints[i].y * spawnPoints[i].y)) 
            bestSpawn = i;
    GameManagerEntity.player.spawnPoint.x = (float)spawnPoints->x;
    GameManagerEntity.player.spawnPoint.y = (float)spawnPoints->y;
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
    for (std::vector <Object>::iterator it = GameManagerEntity.objectArr.begin(); it != GameManagerEntity.objectArr.end(); it++) {
        switch (it->label) {
        case wall:
            switch (it->wallColor) {
            case noColor:
                writeFile << 0;
                break;
            case red:
                writeFile << 10;
                break;
            case blue:
                writeFile << 11;
                break;
            case green:
                writeFile << 12;
                break;
            case popOff:
                writeFile << 13;
                break;
            case popOn:
                writeFile << 14;
                break;
            }
            break;
        case damageZone:
            writeFile << 1;
            break;
        case launch:
            writeFile << 2;
            break;
        case water:
            writeFile << 5;
            break;
        case gravWall:
            writeFile << 16;
            break;
        case items:
            switch (it->itemLabel) {
            case doubleJump:
                writeFile << 3;
                break;
            case dash:
                writeFile << 4;
                break;
            case trophy:
                writeFile << 6;
                break;
            case redKey:
                writeFile << 7;
                break;
            case blueKey:
                writeFile << 8;
                break;
            case greenKey:
                writeFile << 9;
                break;
            case popControl:
                writeFile << 15;
                break;
            }
            break;
        }
        writeFile << ' ' << it->x << ' ' << it->y << ' ' << it->width << ' ' << it->height << ' ' << it->rotation << '\n';
    }
    for (int i = 0; i < spawnPointsNum; i++) 
        writeFile << 99 << ' ' << spawnPoints[i].x << ' ' << spawnPoints[i].y << ' ' << '\n';
    if (GameManagerEntity.announceText.length() != 0) 
        writeFile << 98 << GameManagerEntity.announceText;
    writeFile.close();
}

void saveCampaignState(GameManager GameManagerEntity, MapManager MapManagerEntity) {
    if (MapManagerEntity.campaginType == test) 
        return;
    std::ofstream writeFile;
    if (MapManagerEntity.campaginType == campaign)
        writeFile.open("Maps/Campaign/CampaignSave.txt");
    writeFile << GameManagerEntity.player.spawnPoint.x << ' ' << GameManagerEntity.player.spawnPoint.y << '\n';
    writeFile << MapManagerEntity.row << ' ' << MapManagerEntity.col << ' ' << GameManagerEntity.totalTime << '\n';
    writeFile << GameManagerEntity.player.allowedJumps << ' ' << ((GameManagerEntity.player.allowedDash == true) ? 1 : 0) << ' ' << ((GameManagerEntity.player.hasRed == true) ? 1 : 0) << ' ' << ((GameManagerEntity.player.hasBlue == true) ? 1 : 0) << ' ' << ((GameManagerEntity.player.hasGreen == true) ? 1 : 0) << ' ' << ((GameManagerEntity.player.hasPopControl == true) ? 1 : 0) << '\n';
    writeFile.close();
}

void MapManager::loadCampaignState(GameManager& GameManagerEntity) {
    std::string mapData;
    std::ifstream readFile;
    int tempValue;
    if (campaginType == test)
        return;
    if (campaginType == campaign) 
        readFile.open("Maps/Campaign/CampaignSave.txt");
    if (!readFile.good()) 
        return;
    std::getline(readFile, mapData);
    std::size_t pos = mapData.find(' ');
    std::size_t prevPos = 0;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.spawnPoint.x = (float) tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.spawnPoint.y = (float) tempValue;
    std::getline(readFile, mapData);
    getData(pos, prevPos, mapData, tempValue);
    row = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    col = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.totalTime = tempValue;
    std::getline(readFile, mapData);
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.allowedJumps = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.allowedDash = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.hasRed = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.hasBlue = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.hasGreen = tempValue;
    getData(pos, prevPos, mapData, tempValue);
    GameManagerEntity.player.hasPopControl = tempValue;
    readFile.close();
}

void getData(std::size_t& pos, std::size_t& prevPos, std::string& mapData, int& tempValue) {
    pos = mapData.find(' ', prevPos);
    tempValue = std::stoi(mapData.substr(prevPos, pos));
    prevPos = pos + 1;
}