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
	int dirCd;
	bool attack;
	bool alive;
	sf::Time respawnTime;
	sf::Clock respawnClock;
	
	friend sf::Packet& operator <<(sf::Packet&, const Enemy&); // Send operator.
public:
	Enemy();
	~Enemy();
	void init(std::string, float, float);
	void resetSpd(float, float, float, float, int); // Take player position and size and match with colideable object position and size (considering speed ofc.) if they are colideing, set speed to 0. Used in above function.
	void update(std::vector<std::vector<float>> *);
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