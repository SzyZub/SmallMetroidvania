#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>
#include <string>

typedef enum gameType {
	campaign = 0,
	customCampaign,
	test
}gameType;

class MapManager {
public:
	int row, col;
	gameType type;
	MapManager();
	void changeType(gameType temp);
	void deloadmap(GameManager& temp);
	bool loadmap(GameManager& temp);
	void savemap(GameManager temp, Vector2 spawnPoints[4], int spawnPointsNum);
};

void fillValues(int& val1, int& val2, int& val3, int& val4, int& val5, std::size_t& pos, std::size_t& prevPos, std::string& mapData);
void saveCampaignSate(GameManager temp, MapManager MM);