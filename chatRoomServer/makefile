objects = chatRoomServer.o main.o

chatRoomServer:$(objects)
	g++ -std=c++11 -pthread $(objects) -o chatRoomServer

chatRoomServer.o:chatRoomServer.cpp
	g++ -std=c++11 -pthread -c chatRoomServer.cpp

main.o:main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm chatRoomServer $(objects)
