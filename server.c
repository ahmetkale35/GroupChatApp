#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int client_sockets[MAX_CLIENTS] = {0};
    int max_sd, sd, activity, addrlen, i, j, valread;

    // Winsock ba�latma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Sunucu soketi olu�turma
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    // Sunucu adres ayarlar�
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Soketi ba�lama
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Dinlemeye ba�lama
    if (listen(server_socket, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    addrlen = sizeof(client_addr);

    while (1) {
        // Soket setini s�f�rla
        FD_ZERO(&readfds);

        // Sunucu soketini sete ekle
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        // �stemci soketlerini sete ekle
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        // Aktiviteleri bekle
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity == SOCKET_ERROR) {
            printf("Select error\n");
            break;
        }

        // Yeni bir ba�lant�
        if (FD_ISSET(server_socket, &readfds)) {
            client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
            if (client_socket == INVALID_SOCKET) {
                printf("Accept failed\n");
                continue;
            }

            printf("New connection: IP %s\n", inet_ntoa(client_addr.sin_addr));

            // �stemci soketini kaydet
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = client_socket;
                    break;
                }
            }
        }

        // Mevcut istemcilerden mesaj al ve ilet
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                valread = recv(sd, buffer, BUFFER_SIZE, 0);

                if (valread <= 0) {
                    // Ba�lant� kapat�ld�
                    printf("Client disconnected\n");
                    closesocket(sd);
                    client_sockets[i] = 0;
                } else {
                    // Mesaj al�nd�
                    buffer[valread] = '\0';
                    printf("Received: %s\n", buffer);

                    // Mesaj� di�er istemcilere g�nder
                    for (j = 0; j < MAX_CLIENTS; j++) {
                        if (client_sockets[j] != 0 && client_sockets[j] != sd) {
                            send(client_sockets[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    // Sunucuyu kapatma
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

