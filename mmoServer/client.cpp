#include "client.h"

Client::Client(){
	mapId = 0; // 0 == Starting zone.
	id = 0;
	x = 0;
	y = 0;
	spdX = 0;
	spdY = 0;
	dir = 4;
	mouseButton = 0;
	attackCd = 0;
	attack = false;
	self = new sf::TcpSocket;
}

Client::~Client(){

}

void Client::init(sf::TcpSocket *data){
	self = data;
}

void Client::initId(int data){
	id = data;
	//std::cout << charId << std::endl;
}

void Client::checkAvailableDirections(std::vector<std::vector<float>> *obstacles){
	spdX = 0;
	spdY = 0;
	if (dir == 0) spdY = -2;
	if (dir == 1) spdX = 2;
	if (dir == 2) spdY = 2;
	if (dir == 3) spdX = -2;

	for (unsigned i = 0; i < obstacles->size(); i++){
		resetSpd(obstacles->at(i)[0] * 32, obstacles->at(i)[1] * 32, obstacles->at(i)[2] * 32, obstacles->at(i)[3] * 32); // 32 = block size;
	}
}

void Client::resetSpd(float x2, float y2, float width2, float height2){ 
	if (dir == 0  // TOP
		&& x <= x2 + width2
		&& x2 <= x + 32
		&& y - 2 <= y2 + height2
		&& y2 <= y - 2 + 32){
		dir = 4;
		spdY = 0;
	}

	if (dir == 1 // RIGHT
		&& x + 2 <= x2 + width2
		&& x2 <= x + 2 + 32
		&& y <= y2 + height2
		&& y2 <= y + 32) {
		dir = 4;
		spdX = 0;
	}

	if (dir == 2 // DOWN
		&& x <= x2 + width2
		&& x2 <= x + 32
		&& y + 2 <= y2 + height2
		&& y2 <= y + 2 + 32){
		dir = 4;
		spdY = 0;
	}

	if (dir == 3 // LEFT
		&& x - 2 <= x2 + width2
		&& x2 <= x - 2 + 32
		&& y <= y2 + height2
		&& y2 <= y + 32){
		dir = 4;
		spdX = 0;
	}
}

void Client::update(){
	attack = false;
	if (mouseButton == 1 && !attackCd){
		mouseButton = 0;
		attackCd = 1;
		attack = true;
	}
	if (attackCd){
		attackCd++;
		if (attackCd > 40) attackCd = 0;
	}
	x += spdX;
	y += spdY;
}

std::string Client::getStats(){
	std::stringstream ss;
	ss << dir;
	return ss.str();
}

sf::TcpSocket* Client::getSocket(){
	return self;
}

float Client::getX(){
	return x;
}

float Client::getY(){
	return y;
}

int Client::getId(){
	return id;
}

sf::Packet& operator <<(sf::Packet& packet, const Client& client)
{
	return packet << client.id << client.x << client.y << client.dir << client.attack;
}

sf::Packet& operator >>(sf::Packet& packet, Client& client)
{
	return packet >> client.dir >> client.mouseButton;
}


/*
isColidingSite = function(rect1,rect2){
return rect1.x <= rect2.x+rect2.width
&& rect2.x <= rect1.x+rect1.width
&& rect1.y <= rect2.y + rect2.height
&& rect2.y <= rect1.y + rect1.height;
}
*/