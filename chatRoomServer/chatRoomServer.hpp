#ifndef _CHATROOMSERVER_H_
#define _CHATROOMSERVER_H_

#include <iostream>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>
#include <string>
#include <ifaddrs.h>
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>

#define MAX_CONN 5 //maximum connections at the same time
#define BUFFER_LENGTH 1024 //buffer size for read and write


class chatRoomServer{
	public:
		
		int server_sockid;
		bool RUN;
		std::vector<int> sockfd;
		std::mutex sock_lock;
		
		chatRoomServer(int SOCK_PORT);
		int serverHandler();
		int clientHandler(int sockfd, char* c_addr);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
};

#endif