# Real-Time Group Chat Application

This repository contains a real-time group chat application consisting of a server written in C and a client implemented for Windows systems. The application allows multiple users to connect and exchange messages in real time.

## Features

- Real-time communication between clients.
- Handles up to 10 simultaneous clients.
- Displays timestamps and usernames for messages.
- Cross-platform server functionality.

## Prerequisites

To run this project, ensure you have the following installed:

- **Windows OS** for the client.
- A **compiler for C code** (e.g., GCC or Visual Studio).
- **Winsock2 library** (included with Windows).

## Files

- `server.c`: The server-side program.
- `client.c`: The client-side program.

## Setup Instructions

### 1. Compile the Server and Client

#### Server

1. Open a terminal/command prompt.
2. Compile the `server.c` file:

   ```bash
   gcc -o server server.c -lws2_32
