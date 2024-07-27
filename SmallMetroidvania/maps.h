#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>
#include <string>

typedef enum CampaignType {
	campaign = 0,
	customCampaign,
	test
}CampaignType;

class MapManager {
public:
	int row, col;
	CampaignType campaginType;
	MapManager();
	void deloadmap(GameManager& GameManagerEntity);
	bool loadmap(GameManager& GameManagerEntity);
	void savemap(GameManager GameManagerEntity, Vector2 spawnPoints[4], int spawnPointsNum);
};

void fillValues(int& val1, int& val2, int& val3, int& val4, int& val5, std::size_t& pos, std::size_t& prevPos, std::string& mapData);
void saveCampaignSate(GameManager GameManagerEntity, MapManager MapManagerEntity);