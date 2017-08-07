#include "player.h"

Player::Player(){
	mapId = 0; // 0 == Starting zone.
	id = 0;
	x = 0;
	y = 0;
	spdX = 0;
	spdY = 0;
	dir = 4;
	lastDir = dir;
	mouseButton = 0;
	attackCd = 0;
	attack = false;
	sAttack = false;
	self = new sf::TcpSocket;
	maxHp = 500;
	currHp = maxHp;
	inputIndex = 0;
	dealedInput = 0;

	inputsQueue.resize(0);
}

Player::~Player(){

}

void Player::init(sf::TcpSocket *data){
	self = data;
}

void Player::initId(int data){
	id = data;
	//std::cout << charId << std::endl;
}

void Player::resetSpd(float x2, float y2, float width2, float height2, int direcrtion){
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

void Player::update(std::vector<std::vector<float>> *obstacles){
	if (currHp <= 0) restart();
	dir = 4;
	for (unsigned i = 0; i < inputsQueue.size(); i++){
		//std::cout << attackCd << " , " << inputsQueue[i].mouse << std::endl;
		spdX = 0;
		spdY = 0;
		if (inputsQueue[i].keyboard == 0) spdY = -2;
		if (inputsQueue[i].keyboard == 1) spdX = 2;
		if (inputsQueue[i].keyboard == 2) spdY = 2;
		if (inputsQueue[i].keyboard == 3) spdX = -2;
		if (!mouseButton) mouseButton = inputsQueue[i].mouse;
		for (unsigned j = 0; j < obstacles->size(); j++){
			resetSpd(obstacles->at(j)[0] * 32, obstacles->at(j)[1] * 32, obstacles->at(j)[2] * 32, obstacles->at(j)[3] * 32, inputsQueue[i].keyboard); // 32 = block size;
		}
		if (inputsQueue[i].keyboard != 4) dir = inputsQueue[i].keyboard;
		x += spdX;
		y += spdY;
		dealedInput = inputsQueue[i].index;
	}
	if (dir != 4)lastDir = dir;
	inputsQueue.resize(0);
	if (mouseButton == 1 && !attackCd){
		attackCd = 1;
		attack = true;
	}
	if (attackCd){
		attackCd++;
		if (attackCd > 10) attackCd = 0;
	}
	mouseButton = 0;
	sAttack = attack;
}

void Player::harm(int damage){
	currHp -= damage;
}

void Player::restart(){
	currHp = maxHp;
	x = 0;
	y = 0;
	attack = false;
	attackCd = 0;
}

void Player::rAttacking(){
	attack = false;
}

std::string Player::getStats(){
	std::stringstream ss;
	ss << dir;
	return ss.str();
}

sf::TcpSocket* Player::getSocket(){
	return self;
}

float Player::getX(){
	return x;
}

float Player::getY(){
	return y;
}

int Player::getId(){
	return id;
}

int Player::getDir(){
	return lastDir;
}

bool Player::attacking(){
	return attack;
}

sf::Packet& operator <<(sf::Packet& packet, const Player& player)
{
	return packet << player.id << player.x << player.y << player.dir << player.sAttack << player.maxHp << player.currHp;
}

sf::Packet& operator <(sf::Packet& packet, const Player& player)
{
	return packet << player.dealedInput << player.x << player.y << player.sAttack << player.maxHp << player.currHp;
}

sf::Packet& operator >>(sf::Packet& packet, Player& client)
{
	//return packet >> client.inputIndex >> client.dir >> client.mouseButton;
	InputS tmp;
	packet >> tmp.index;
	packet >> tmp.keyboard;
	packet >> tmp.mouse;
	client.inputsQueue.push_back(tmp);
	return packet;
}


/*
isColidingSite = function(rect1,rect2){
return rect1.x <= rect2.x+rect2.width
&& rect2.x <= rect1.x+rect1.width
&& rect1.y <= rect2.y + rect2.height
&& rect2.y <= rect1.y + rect1.height;
}
*/