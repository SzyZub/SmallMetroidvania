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
private:
public:
	int row, col;
	gameType type;
	MapManager();
	void changeType(gameType temp);
	void deloadmap(GameManager& temp);
	bool loadmap(GameManager& temp);
	void savemap(GameManager temp, Vector2 spawnPoints[4], int spawnPointsNum);
};