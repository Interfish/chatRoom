# ChatRoom
This is a personal project by [Interfish](https://github.com/Interfish). I developed a chatroom app where anyone who get connected 
can send text message(limited for ASCII) to everyone in the room. This app use TCP/IP and Socket.

Server side is written in C++. Once running, the server will first try to create a socket and bind and listen to port specified by
start-up argument. Then server will create a handler thread to catch incoming connnection request, while the main thread keep on listening
command from the server side. Once a connection established, handler will create a client-handler thread. The client-handler thread will
listen on incomming data, and boradcast it to every other client-handlers. The maximum number of clients is set to 5 by default.

Client side is written in Java. Once running, the client will create two threads. Read thread will listen on incomming data and print them
on the screen. Write thread will listen on standard input stream for user input and send them through socket. 

prototype folder is only doing for learning purpose. There is a python server and a Java client. It does not affect on the app.


