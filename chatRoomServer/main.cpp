#include "chatRoomServer.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]){
	if(argc == 1){
		std::cout << "Please enter port number as an argument" 
		          << std::endl;
		return 1;
	}
	int port = std::atoi(argv[1]);
	chatRoomServer chatRoom(port);
	return 0;
}