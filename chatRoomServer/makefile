objects = chatRoomServer.o main.o

chatRoom:$(objects)
	g++ -std=c++11 -pthread $(objects) -o chatRoom

chatRoomServer.o:chatRoomServer.cpp
	g++ -std=c++11 -pthread -c chatRoomServer.cpp

main.o:main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm chatRoom $(objects)