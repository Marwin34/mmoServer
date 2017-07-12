#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

class Client {
	sf::TcpSocket* self;
	float x, y;
	float spdX, spdY;
	int dir;
	int lastDir;
	unsigned int mouseButton;
	int id;
	int currHp;
	int maxHp;
	int mapId; // Id of the map where client is.
	int attackCd; // Cooldown of simple attack;
	bool attack;

	friend sf::Packet& operator <<(sf::Packet&, const Client&); // Send operator.
	friend sf::Packet& operator >>(sf::Packet&, Client&); // Receive operator.

public:
	Client();
	~Client();
	void init(sf::TcpSocket*);
	void initId(int);
	void checkAvailableDirections(std::vector<std::vector<float>>*); // Determine whether plauyer can go in choosed direction. Call it alwasy before UPDATE!!!! 
	void resetSpd(float, float, float, float); // Take player position and size and match with colideable object position and size (considering speed ofc.) if they are colideing, set speed to 0. Used in above function.
	void update();
	void harm(int);
	void restart();
	float getX();
	float getY();
	std::string getStats();
	sf::TcpSocket* getSocket();
	int getId();
	int getDir();
	bool attacking();
};