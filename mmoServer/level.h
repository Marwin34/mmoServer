#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <SFML\Graphics.hpp>

#pragma once

class Level{

	int width;
	int height;

	std::vector<std::vector<float>> obstacles;

public:
	Level();
	~Level();
	bool load(std::string);
	std::vector<std::vector<float>> getObstacles();
};