#include "level.h"

Level::Level(){
	width = 0;
	height = 0;

	obstacles.resize(0);
}

Level::~Level(){
	width = 0;
	height = 0;
}

bool Level::load(std::string path){
	std::fstream file(path, std::ios::in);
	if (file){
		std::string linia;
		int cnt = 0;

		std::string tmp;
		std::string tmp2;
		std::istringstream stream("");

		while (!file.eof()){
			std::getline(file, linia);
			if (linia.find("width") != std::string::npos && linia.find("width") == 0) { // wczytanie width
				tmp = linia.substr(linia.find("=") + 1, linia.length() - linia.find("=") - 1);
				stream.clear();
				stream.str(tmp);
				stream >> width;
			}

			if (linia.find("height") != std::string::npos && linia.find("height") == 0) { // wczytanie height
				tmp = linia.substr(linia.find("=") + 1, linia.length() - linia.find("=") - 1);
				stream.clear();
				stream.str(tmp);
				stream >> height;
			}

			if (linia.find("type=colider") != std::string::npos && linia.find("type=colider") == 0){ // If there is any colider on the map, read it.
				std::vector<float> tmpObstacle; // Create temporary obstacle
				while (1){
					file >> tmp;
					if (tmp.find("location") != std::string::npos && tmp.find("location") == 0){ // Fin its location and read it to memory.
						tmp = tmp.substr(tmp.find("=") + 1, tmp.length() - tmp.find("=") - 1);
						stream.clear();
						stream.str(tmp);
						while (tmp.length()){
							if (tmp.find(",") != std::string::npos){ // For 3 first stats.
								tmp2 = tmp.substr(0, tmp.find(","));
								stream.clear();
								stream.str(tmp2);
								float tmpInt;
								stream >> tmpInt;
								tmpObstacle.push_back(tmpInt);
								tmp = tmp.substr(tmp.find(",") + 1, tmp.length() - tmp.find(",") - 1);
							}
							else { // For last stat.
								stream.clear();
								stream.str(tmp);
								float tmpInt;
								stream >> tmpInt;
								tmpObstacle.push_back(tmpInt);
								tmp = ""; // Set tmp string to empty.
							}
						}
						break;
					}
				}
				if (tmpObstacle.size() == 4) obstacles.push_back(tmpObstacle); // If succesfully readed all 4 obstacle statistics, add it to vector.
			}
		}
		file.close();

		/*for (unsigned i = 0; i < obstacles.size(); i++){
		std::cout << obstacles[i][0] << " , " << obstacles[i][1] << " , " << obstacles[i][2] << " , " << obstacles[i][3] << std::endl;
		}*/
		return true;
	}
	else {
		std::cout << "Unable to open file!!! " << path << std::endl;
	}
	return false;
}

std::vector<std::vector<float>> Level::getObstacles(){
	return obstacles;
}