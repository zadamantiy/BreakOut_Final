#include "LevelLoader.h"
#include <fstream>

void LevelLoader::Load_level(std::string lvlName)
{
	std::ifstream fin("Levels\\" + lvlName + ".txt");

	levelName = lvlName;

	fin >> h >> w;

	int tmp;
	for (auto i = 0; i < h; i++)
	for (auto j = 0; j < w; j++)
	{
		fin >> tmp;
		level.push_back(tmp);
	}
}

int LevelLoader::getType(int n) const
{
	return level[n];
}

std::string LevelLoader::getName() const
{
	return levelName;
}