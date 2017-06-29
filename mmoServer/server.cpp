#include "server.h"

Server::Server() : r_thread(&Server::receive, this){
	listener.listen(55001); // Bind listener to the port.
	selector.add(listener); // Put listener to the selector.
	running = true;
	mainTimer = sf::Time::Zero; // Initialize the mainTimer.
	lastUpdate = sf::Time::Zero; // initialize the lastUpdate timer;

	mapsInitialization();
}

Server::~Server(){

}

void Server::run(){
	srand((unsigned)time(NULL));
	r_thread.launch();
	while (running){
		//receive(); // Receive data any time something is waiting.

		mainTimer = mainClock.getElapsedTime(); // Get the main time;

		if (mainTimer.asMilliseconds() - lastUpdate.asMilliseconds() >= 20){ // Update scene and send data only every 50 milliseconds;
			//std::cout << timer << std::endl;
			for (unsigned i = 0; i < maps.size(); i++){
				for (unsigned j = 0; j < maps[i].clients.size(); j++){
					maps[i].clients[j]->checkAvailableDirections(&maps[i].level.getObstacles());
					maps[i].clients[j]->update();
				}
			}

			send();
			lastUpdate = mainTimer;
		}
	}
}

void Server::receive(){
	while (running){
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				//sf::TcpSocket* client = new sf::TcpSocket;
				Client* client = new Client;
				if (listener.accept(*client->getSocket()) == sf::Socket::Done)
				{
					client->initId(rand()); // Set randomly generated ID for client.
					std::string type = "INIT";
					sf::Packet packet;
					packet << type << client->getId();
					client->getSocket()->send(packet); // Send id to client;
					// Add the new client to the clients list
					maps[0].clients.push_back(client);
					// Add the new client to the selector so that we will
					// be notified when he sends something 
					selector.add(*client->getSocket());

					std::cout << "Polaczono z : " << client->getSocket()->getRemoteAddress() << " : " << client->getSocket()->getRemotePort() << " / " << client->getSocket()->getLocalPort() << " " << client->getId() << std::endl;
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					delete client;
				}
			}
			else
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (unsigned i = 0; i < maps.size(); i++){
					for (unsigned int j = 0; j < maps[i].clients.size(); j++){
						Client& client = *maps[i].clients[j];
						if (selector.isReady(*client.getSocket()))
						{
							// The client has sent some data, we can receive it
							sf::Packet packet;
							sf::Socket::Status status = client.getSocket()->receive(packet);
							if (status == sf::Socket::Done)
							{
								packet >> client;
							}
							else if (status == sf::Socket::Disconnected) // If client want to disconnect, remove him from selector and list of clients
							{
								//std::cout << client.getSocket()->getRemoteAddress() << " , " << client.getSocket()->getRemotePort() << " disconnected!" << std::endl;
								selector.remove(*client.getSocket());
								client.getSocket()->disconnect();
								delete &client;
								maps[i].clients.erase(maps[i].clients.begin() + j);
							}
						}
					}
				}
			}
		}
	}
}

void Server::send(){
	for (unsigned i = 0; i < maps.size(); i++){

		std::string type = "DATAS"; // Data packet header.
		sf::Packet packet;
		packet << type << maps[i].clients.size();

		for (unsigned j = 0; j < maps[i].clients.size(); j++){ // Gather all data.
			Client& client = *maps[i].clients[j];
			packet << client;
		}

		for (unsigned j = 0; j < maps[i].clients.size(); j++){ // Send data to players on this map.
			if (maps[i].clients[j]->getSocket()->send(packet) != sf::Socket::Done) continue;
		}
	}
}

void Server::mapsInitialization(){
	Map tmp;
	maps.resize(0);

	if (tmp.level.load("starting.txt")){
		tmp.clients.resize(0);
		maps.push_back(tmp);
	}
}