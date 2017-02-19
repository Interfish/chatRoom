#include "chatRoomServer.hpp"

chatRoomServer::chatRoomServer(int SOCK_PORT){
	int fd_temp;
	struct sockaddr_in s_addr_in;

	chatRoomServer::RUN = true;
	chatRoomServer::server_sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(chatRoomServer::server_sockid==-1){
		std::cout << "create socket failed" << std::endl;
		exit(-1);
	}
	std::cout << "create server socket" << std::endl;

	memset(&s_addr_in, 0, sizeof(s_addr_in));
	s_addr_in.sin_family = AF_INET;
	s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr_in.sin_port = htons(SOCK_PORT);

	fd_temp = bind(chatRoomServer::server_sockid, (struct sockaddr *)(&s_addr_in), sizeof(s_addr_in));
	if(fd_temp==-1){
		std::cout << "Bind error" << std::endl;
		exit(-1);
	}

	std::cout << "Server ethernet interface IP and local IP:" << std::endl;
	setenv("LANG","C",1);
    FILE * fp = popen("ifconfig", "r");
    if(fp){
        char *p=NULL, *e; size_t n;
        while ((getline(&p, &n, fp) > 0) && p) {
           	if (p = strstr(p, "inet ")) {
                p+=5;
                if (p = strchr(p, ':')) {
                    ++p;
                    if (e = strchr(p, ' ')) {
                        *e='\0';
                        printf("%s\n", p);
                    }
                }
            }
        }
    }
    pclose(fp);
	
	std::cout << "Port number is " << SOCK_PORT << std::endl;

	fd_temp = listen(chatRoomServer::server_sockid, MAX_CONN);
	if(fd_temp==-1){
		std::cout << "Listen error" << std::endl;
		exit(-1);
	}
	std::cout << "Listening on port" << std::endl;

	std::thread server_handler_thread(&chatRoomServer::serverHandler,this);

	std::string command;
	while(1){
		std::cin >> command;
		if(command=="quit")
			chatRoomServer::RUN = false;
			break;
	}
	int ret = shutdown(chatRoomServer::server_sockid, SHUT_RDWR);
	if(ret == -1){
		std::cout << "Shutdown failed" << std::endl;
		exit(-1);
	}
	server_handler_thread.join();
	std::cout << "Server shutting down" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

int chatRoomServer::serverHandler(){
	while(chatRoomServer::RUN){
		struct sockaddr_in client_addr;
		int client_length = sizeof(client_addr);

		std::cout << "server handler is waiting for new connection" << std::endl;
		int temp_sockfd = accept(chatRoomServer::server_sockid, 
		                        (struct sockaddr*)(&client_addr),
		                        (socklen_t *)(&client_length));
		if(temp_sockfd==ENFILE){
			std::cout << "max connection reached" << std::endl;
			continue;
		}
		if(temp_sockfd==-1){
			std::cout << "Connection " << chatRoomServer::sockfd.size() 
					  << " accept error" << " Server may be shutting down"
					  << std::endl;
			return 1;
		}
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, addr, INET_ADDRSTRLEN);
		chatRoomServer::sock_lock.lock();
		chatRoomServer::sockfd.push_back(temp_sockfd);
		chatRoomServer::sock_lock.unlock();

		std::thread client_thread(&chatRoomServer::clientHandler, this, temp_sockfd, addr);
		client_thread.detach();
	}
	return 0;
}

int chatRoomServer::clientHandler(int fd, char* addr){
	std::cout << "Client " << chatRoomServer::sockfd.size()-1<< " : "
		 << addr << " online" << std::endl;
	int recvBytes;
	char data_recv[BUFFER_LENGTH];
	
	while(chatRoomServer::RUN){
		memset(data_recv, 0, BUFFER_LENGTH);
		recvBytes = read(fd, data_recv, BUFFER_LENGTH);
		if(recvBytes==0){
			std::cout << "Server may be closing" << std::endl;
			break;
		}
		if(recvBytes==-1){
			std::cout << "Read error" << " Client my be closing "<< std::endl;
			break;
		}
		if(strcmp(data_recv,"quit")==0)
			break;
		std::string text(data_recv);
		std::string ip_address(addr);
		text = "--------------------\n" + ip_address + ":\n" + text + "\n"
		     + "--------------------\n";
		const char* data_send = text.c_str();
		std::cout << text << std::endl;

		chatRoomServer::sock_lock.lock();
		for(int i=0;i<chatRoomServer::sockfd.size();i++){
			if(fd==chatRoomServer::sockfd[i])	continue;
			if(write(chatRoomServer::sockfd[i], data_send, strlen(data_send))==-1){
				std::cout << "Write error" << std::endl;
				break;
			}
		}
		chatRoomServer::sock_lock.unlock();
	}
	std::cout << "shut down client handler.." << std::endl;
	close(fd);
	chatRoomServer::sock_lock.lock();
	for(int i=0;i<chatRoomServer::sockfd.size();i++){
		if(fd==chatRoomServer::sockfd[i])
			chatRoomServer::sockfd.erase(chatRoomServer::sockfd.begin()+i);
	}
	chatRoomServer::sock_lock.unlock();
	std::cout << "Client quit" << std::endl;
	return 0;
}