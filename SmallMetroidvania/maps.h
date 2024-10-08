#pragma once
#include "objects.h"
#include <iostream>
#include <fstream>

typedef enum CampaignType {
	campaign = 0,
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
	void loadCampaignState(GameManager& GameManagerEntity);
};

void fillValues(int& val1, int& val2, int& val3, int& val4, int& val5, std::size_t& pos, std::size_t& prevPos, std::string& mapData);
void saveCampaignState(GameManager GameManagerEntity, MapManager MapManagerEntity);
void getData(std::size_t& pos, std::size_t& prevPos, std::string& mapData, int& tempValue);