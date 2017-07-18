#include <iostream>
#include <SFML/Network.hpp>

#include "level.h"
#include "client.h"
#include "enemy.h"

class Server {
	sf::TcpListener listener; // Create listener for listening for new connections.
	sf::SocketSelector selector;  // Create selector for sockets.
	bool running;
	int serverTick;
	sf::Clock mainClock; // Creat mainClock of the client loop.
	sf::Clock fpsClock;
	sf::Time mainTimer; // Create mainTimer of the client loop.
	sf::Time lastUpdate; // Time of the last update of the client.
	sf::Time msgTimer; // Create mainTimer of the client loop.
	sf::Time lastMsg; // Time of the last update of the client.
	sf::Window window;

	struct DamageArea {
		float x, y, width, height;
		int damage, ttl;
		int originId;
	};

	struct Map {
		Level level;
		std::vector<Client*> clients; // Make a list of clients, for each map.
		std::vector<Enemy> enemies;
		std::vector<DamageArea> damageAreas; // Harmful areas ont he map. Spells, sword attacks itp.
	};

	std::vector<Map> maps;

	sf::Thread r_thread;
public:
	Server();
	~Server();
	void run();
	void receive();
	void send();
	void mapsInitialization();
	void damageDealer();
};