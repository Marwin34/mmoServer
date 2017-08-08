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

void Enemy::update(){
	if (alive && currHp <= 0) {
		alive = false;
		respawnClock.restart();
	}
	if (!alive && respawnClock.getElapsedTime().asMilliseconds() >= respawnTime.asMilliseconds()) {
		respawn();
	}
}

void Enemy::harm(int damage){
	currHp -= damage;
}

void Enemy::respawn(){
	x = oX;
	y = oY;
	currHp = maxHp;
	alive = true;
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