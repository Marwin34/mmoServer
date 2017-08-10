#include "enemy.h"

Enemy::Enemy(){
	oX = 0;
	oY = 0;
	x = oX;
	y = oY;
	maxHp = 0;
	currHp = maxHp;
	damage = 0;
	range = 0;
	dir = 0;
	dirCd = 0;
	respawnTime = sf::Time::Zero;
	attack = false;
	alive = false;
}

Enemy::~Enemy(){

}

void Enemy::init(std::string name, float startX, float startY){
	if (name == "Orc"){
		oX = startX;
		oY = startY;
		x = oX;
		y = oY;
		maxHp = 100;
		currHp = maxHp;
		respawnTime = sf::seconds(5);
		range = 16;
		alive = true;
	}
}

void Enemy::resetSpd(float x2, float y2, float width2, float height2, int direcrtion){
	if (direcrtion == 0  // TOP
		&& x <= x2 + width2
		&& x2 <= x + 32
		&& y - 2 <= y2 + height2
		&& y2 <= y - 2 + 32){
		dir = 4;
		spdY = 0;
	}

	if (direcrtion == 1 // RIGHT
		&& x + 2 <= x2 + width2
		&& x2 <= x + 2 + 32
		&& y <= y2 + height2
		&& y2 <= y + 32) {
		dir = 4;
		spdX = 0;
	}

	if (direcrtion == 2 // DOWN
		&& x <= x2 + width2
		&& x2 <= x + 32
		&& y + 2 <= y2 + height2
		&& y2 <= y + 2 + 32){
		dir = 4;
		spdY = 0;
	}

	if (direcrtion == 3 // LEFT
		&& x - 2 <= x2 + width2
		&& x2 <= x - 2 + 32
		&& y <= y2 + height2
		&& y2 <= y + 32){
		dir = 4;
		spdX = 0;
	}
}

void Enemy::update(std::vector<std::vector<float>> *obstacles){
	if (alive && currHp <= 0) {
		alive = false;
		respawnClock.restart();
	}
	if (!alive && respawnClock.getElapsedTime().asMilliseconds() >= respawnTime.asMilliseconds()) {
		respawn();
	}
	if (alive){
		if (dirCd){
			dirCd++;
			if (dirCd > 10) dirCd = 0;
		}
		else {
			dir = ((rand() + 1) % 200) / 50;
			dirCd++;
		}
		spdY = 0;
		spdX = 0;
		if (dir == 0) spdY = -2;
		if (dir == 1) spdX = 2;
		if (dir == 2) spdY = 2;
		if (dir == 3) spdX = -2;
		for (unsigned j = 0; j < obstacles->size(); j++){
			resetSpd(obstacles->at(j)[0] * 32, obstacles->at(j)[1] * 32, obstacles->at(j)[2] * 32, obstacles->at(j)[3] * 32, dir); // 32 = block size;
		}
	}
	x += spdX;
	y += spdY;
}

void Enemy::harm(int damage){
	currHp -= damage;
}

void Enemy::respawn(){
	x = oX;
	y = oY;
	currHp = maxHp;
	alive = true;
	dirCd = 0;
	dir = 4;
}

float Enemy::getRange(){
	return range;
}

float Enemy::getX(){
	return x;
}

float Enemy::getY(){
	return y;
}

bool Enemy::isAttacking(){
	return attack;
}

bool Enemy::isAlive(){
	return alive;
}

sf::Time Enemy::getRespawnTime(){
	return respawnTime;
}

sf::Packet& operator <<(sf::Packet& packet, const Enemy& enemy)
{
	return packet << enemy.id << enemy.x << enemy.y << enemy.dir <<enemy.attack << enemy.maxHp << enemy.currHp;
}

std::vector<Enemy> enemiesInit(std::string mapName){
	std::vector<Enemy> tmp;
	tmp.resize(0);
	if (mapName == "starting"){
		Enemy tmpEnemy;
		tmpEnemy.init("Orc", 200, 200);
		tmp.push_back(tmpEnemy);
	}
	return tmp;
}