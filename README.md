Real-Time Group Chat Application

This repository contains a real-time group chat application consisting of a server written in C and a client implemented for Windows systems. The application allows multiple users to connect and exchange messages in real time.

Features

Real-time communication between clients.

Handles up to 10 simultaneous clients.

Displays timestamps and usernames for messages.

Cross-platform server functionality.

Prerequisites

To run this project, ensure you have the following installed:

Windows OS for the client.

A compiler for C code (e.g., GCC or Visual Studio).

Winsock2 library (included with Windows).

Files

server.c: The server-side program.

client.c: The client-side program.

Setup Instructions

1. Compile the Server and Client

Server

Open a terminal/command prompt.

Compile the server.c file:

gcc -o server server.c -lws2_32

Client

Open a terminal/command prompt.

Compile the client.c file:

gcc -o client client.c -lws2_32 -lpthread

2. Run the Server

Execute the server program:

./server

The server will listen on port 8080 for incoming connections.

3. Run the Client

Execute the client program:

./client

Enter the server's IP address (10.26.244.62) when prompted.

Enter your username and start chatting.

Notes

Ensure the server and clients are on the same network.

The server listens on PORT 8080 by default; you can modify this in the source code if needed.

The maximum number of clients is 10. This limit can be increased by changing the MAX_CLIENTS constant in server.c.

Contributing

Feel free to submit issues or pull requests to improve this project.

License

This project is open-source and licensed under the MIT License.
