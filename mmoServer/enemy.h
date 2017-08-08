#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

class Enemy{
	float oX, oY;
	float x, y;
	float spdX, spdY;
	float range;
	int maxHp;
	int currHp;
	int damage;
	int id;
	int dir; 
	bool attack;
	bool alive;
	sf::Time respawnTime;
	sf::Clock respawnClock;
	
	friend sf::Packet& operator <<(sf::Packet&, const Enemy&); // Send operator.
public:
	Enemy();
	~Enemy();
	void init(std::string, float, float);
	void update();
	void harm(int);
	void respawn();
	float getRange();
	float getX();
	float getY();
	bool isAttacking();
	bool isAlive();
	sf::Time getRespawnTime();
};

std::vector<Enemy> enemiesInit(std::string);