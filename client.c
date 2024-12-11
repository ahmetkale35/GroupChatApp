#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Zaman formatlama fonksiyonu
void get_current_time(char *time_str, size_t size) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    strftime(time_str, size, "%Y-%m-%d %H:%M:%S", local_time);
}

// Gelen mesajlar� almak i�in i� par�ac���
void *receive_messages(void *socket) {
    SOCKET sock = *(SOCKET *)socket;
    char buffer[BUFFER_SIZE];
    int valread;

    while (1) {
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            buffer[valread] = '\0';

            // Gelen mesaj� ekrana yazd�r
            printf("\n%s\n", buffer); // Mesaj� yazd�r
            printf("\n"); // Mesajlar aras�nda bo�luk b�rak

            // Kullan�c� giri�ini yeniden g�ster
            printf("[You] Enter your message: ");
            fflush(stdout); // ��kt�y� hemen temizle
        }
    }

    return NULL;
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char time_buffer[64];
    char message_with_time[BUFFER_SIZE + 64];
    char username[50]; // Kullan�c� ad�
    pthread_t recv_thread;

    // Winsock ba�latma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Soket olu�turma
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    // Sunucu adres ayarlar�
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.103.48"); // Sunucu IP adresi

    // Ba�lant�y� kurma
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server\n");

    // Kullan�c� ad� iste�i
    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Yeni sat�r karakterini kald�r

    // Kullan�c� ad�n� sunucuya g�nder
    send(sock, username, strlen(username), 0);

    // Mesajlar� almak i�in i� par�ac��� ba�lat
    pthread_create(&recv_thread, NULL, receive_messages, &sock);

    while (1) {
        // Kullan�c�dan mesaj al
        printf("[You] Enter your message: ");
        fflush(stdout); // Terminali temizlemeden �nce g�ster

        fgets(buffer, BUFFER_SIZE, stdin);

        // Zaman damgas� ekle
        get_current_time(time_buffer, sizeof(time_buffer));
        snprintf(message_with_time, sizeof(message_with_time), "[%s - %s] %s", time_buffer, username, buffer);

        // Mesaj� sunucuya g�nder
        send(sock, message_with_time, strlen(message_with_time), 0);

        // Mesajlar aras�nda bo�luk b�rak
        printf("\n");
    }

    // Soketi kapat
    closesocket(sock);
    WSACleanup();

    return 0;
}

