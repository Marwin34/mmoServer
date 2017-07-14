#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

class Enemy{
	float x, y;
	float spdX, spdY;
	int maxHp;
	int currHp;
	int damage;
	int id;
	int dir;
	sf::Time respawnTime;
	float range;
	bool attack;

	friend sf::Packet& operator <<(sf::Packet&, const Enemy&); // Send operator.
public:
	Enemy();
	~Enemy();
	void init(std::string, float, float);
	void update();
	void harm(int);
	float getRange();
	float getX();
	float getY();
	bool isAttacking();
	sf::Time getRespawnTime();
};

std::vector<Enemy> enemiesInit(std::string);