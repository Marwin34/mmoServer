#include <iostream>
#include <SFML/Network.hpp>

#include "level.h"
#include "client.h"

class Server {
	sf::TcpListener listener; // Create listener for listening for new connections.
	sf::SocketSelector selector;  // Create selector for sockets.
	bool running;
	sf::Clock mainClock; // Creat mainClock of the client loop.
	sf::Time mainTimer; // Create mainTimer of the client loop.
	sf::Time lastUpdate; // Time of the last update of the client.

	struct Map {
		Level level;
		std::vector<Client*> clients; // Make a list of clients, for each map.
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
};