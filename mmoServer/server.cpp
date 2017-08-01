#include "server.h"

Server::Server() : r_thread(&Server::receive, this){
	listener.listen(55001); // Bind listener to the port.
	selector.add(listener); // Put listener to the selector.
	running = true;
	serverTick = 0;
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
		mainTimer = mainClock.getElapsedTime(); // Get the main time;

		if (mainTimer.asMilliseconds() - lastUpdate.asMilliseconds() >= 100){ // Update scene and send data only every 50 milliseconds;
			//std::cout << serverTick << std::endl;
			
			for (unsigned i = 0; i < maps.size(); i++){
				for (unsigned j = 0; j < maps[i].enemies.size(); j++){
					maps[i].enemies[j].update();
				}
			}
			for (unsigned i = 0; i < maps.size(); i++){
				for (unsigned j = 0; j < maps[i].clients.size(); j++){
					maps[i].clients[j]->update(&maps[i].level.getObstacles());
				}
			}
			damageDealer();
			lastUpdate = mainTimer;
			serverTick++;
			std::cout << "Updates Per Second : " << 1.f / fpsClock.getElapsedTime().asSeconds() << std::endl;
			send();
			fpsClock.restart();
		}		
		sf::sleep(sf::milliseconds(10));
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
		for (unsigned j = 0; j < maps[i].clients.size(); j++){ 
			packet.clear();
			packet << type << maps[i].clients.size() - 1 << maps[i].enemies.size(); // Insert count of enemies and players (excluded player which is contolled by this client).
			Client& client = *maps[i].clients[j];
			packet < client; // Insert data abaut player controlled by this client.
			for (unsigned k = 0; k < maps[i].clients.size(); k++){ // Gather data about players.
				if (maps[i].clients[k]->getId() == client.getId()) continue;
				packet << *maps[i].clients[k];
			}
			for (unsigned k = 0; k < maps[i].enemies.size(); k++){ // Gather data abaut enemies.
				packet << maps[i].enemies[k];
			}
			if (maps[i].clients[j]->getSocket()->send(packet) != sf::Socket::Done) continue; // Send data to players on this map.
		}
	}
}

void Server::mapsInitialization(){
	Map tmp;
	maps.resize(0);
	if (tmp.level.load("starting.txt")){
		tmp.clients.resize(0);
		tmp.damageAreas.resize(0);
		tmp.enemies = enemiesInit("starting");
		maps.push_back(tmp);
	}
}

void Server::damageDealer(){ // Obsolete, not used currently!!!!
	for (unsigned i = 0; i < maps.size(); i++){
		for (unsigned j = 0; j < maps[i].clients.size(); j++){
			if (maps[i].clients[j]->attacking()){
				DamageArea tmp;

				if (maps[i].clients[j]->getDir() == 0){
					tmp.x = maps[i].clients[j]->getX();
					tmp.y = maps[i].clients[j]->getY() - 10;
					tmp.width = 32;
					tmp.height = 10;
				}
				if (maps[i].clients[j]->getDir() == 1){
					tmp.x = maps[i].clients[j]->getX() + 32;
					tmp.y = maps[i].clients[j]->getY();
					tmp.width = 10;
					tmp.height = 32;
				}
				if (maps[i].clients[j]->getDir() == 2 || maps[i].clients[j]->getDir() == 4){
					tmp.x = maps[i].clients[j]->getX();
					tmp.y = maps[i].clients[j]->getY() + 32;
					tmp.width = 32;
					tmp.height = 10;
				}
				if (maps[i].clients[j]->getDir() == 3){
					tmp.x = maps[i].clients[j]->getX() - 10;
					tmp.y = maps[i].clients[j]->getY();
					tmp.width = 10;
					tmp.height = 32;
				}
				//std::cout << tmp.x << " , " << tmp.y << std::endl;
				tmp.damage = 44;
				tmp.ttl = 1;
				tmp.originId = maps[i].clients[j]->getId();
				maps[i].damageAreas.push_back(tmp);
				maps[i].clients[j]->rAttacking();
			}
		}
		for (unsigned j = 0; j < maps[i].damageAreas.size(); j++){
			for (unsigned k = 0; k < maps[i].clients.size(); k++){
				if (maps[i].damageAreas[j].x <= maps[i].clients[k]->getX() + 32
					&& maps[i].clients[k]->getX() <= maps[i].damageAreas[j].x + maps[i].damageAreas[j].width
					&& maps[i].damageAreas[j].y <= maps[i].clients[k]->getY() + 32
					&& maps[i].clients[k]->getY() <= maps[i].damageAreas[j].y + maps[i].damageAreas[j].height
					&& maps[i].clients[k]->getId() != maps[i].damageAreas[j].originId){
					maps[i].clients[k]->harm(maps[i].damageAreas[j].damage);
				}
			}
			for (unsigned k = 0; k < maps[i].enemies.size(); k++){
				if (maps[i].damageAreas[j].x <= maps[i].enemies[k].getX() + 32
					&& maps[i].enemies[k].getX() <= maps[i].damageAreas[j].x + maps[i].damageAreas[j].width
					&& maps[i].damageAreas[j].y <= maps[i].enemies[k].getY() + 32
					&& maps[i].enemies[k].getY() <= maps[i].damageAreas[j].y + maps[i].damageAreas[j].height){
					maps[i].enemies[k].harm(maps[i].damageAreas[j].damage);
				}
			}
			maps[i].damageAreas[j].ttl--;
			if (!maps[i].damageAreas[j].ttl) maps[i].damageAreas.erase(maps[i].damageAreas.begin() + j);
		}
	}
}