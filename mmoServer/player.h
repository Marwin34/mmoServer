#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

struct InputS{
	int index;
	int keyboard;
	int mouse;
};

class Player {
	sf::TcpSocket* self;
	float x, y;
	float spdX, spdY;
	int dir;
	unsigned int mouseButton;
	int id;
	int currHp;
	int maxHp;
	int mapId; // Id of the map where client is.
	int attackCd; // Cooldown of simple attack;
	int inputIndex;
	int dealedInput;
	int lastDir;
	bool attack;
	bool sAttack;

	std::vector<InputS> inputsQueue;

	friend sf::Packet& operator <<(sf::Packet&, const Player&); // Send operator.
	friend sf::Packet& operator <(sf::Packet&, const Player&); // Send operator.
	friend sf::Packet& operator >>(sf::Packet&, Player&); // Receive operator.

public:
	Player();
	~Player();
	void init(sf::TcpSocket*);
	void initId(int);
	void resetSpd(float, float, float, float, int); // Take player position and size and match with colideable object position and size (considering speed ofc.) if they are colideing, set speed to 0. Used in above function.
	void update(std::vector<std::vector<float>> *);
	void harm(int);
	void restart();
	void rAttacking();
	float getX();
	float getY();
	std::string getStats();
	sf::TcpSocket* getSocket();
	int getId();
	int getDir();
	bool attacking();
};