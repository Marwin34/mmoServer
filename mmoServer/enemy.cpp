#include "enemy.h"

Enemy::Enemy(){
	x = 0;
	y = 0;
	maxHp = 0;
	currHp = maxHp;
	damage = 0;
	range = 0;
	dir = 0;
	respawnTime = sf::Time::Zero;
	attack = false;
}

Enemy::~Enemy(){

}

void Enemy::init(std::string name, float startX, float startY){
	if (name == "Orc"){
		x = startX;
		y = startY;
		maxHp = 100;
		currHp = maxHp;
		respawnTime = sf::seconds(5);
		range = 16;
	}
}

void Enemy::update(){
	 
}

void Enemy::harm(int damage){
	currHp -= damage;
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