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

// Gelen mesajlarý almak için iþ parçacýðý
void *receive_messages(void *socket) {
    SOCKET sock = *(SOCKET *)socket;
    char buffer[BUFFER_SIZE];
    int valread;

    while (1) {
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            buffer[valread] = '\0';

            // Gelen mesajý ekrana yazdýr
            printf("\n%s\n", buffer); // Mesajý yazdýr
            printf("\n"); // Mesajlar arasýnda boþluk býrak

            // Kullanýcý giriþini yeniden göster
            printf("[You] Enter your message: ");
            fflush(stdout); // Çýktýyý hemen temizle
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
    char username[50]; // Kullanýcý adý
    pthread_t recv_thread;

    // Winsock baþlatma
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Soket oluþturma
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    // Sunucu adres ayarlarý
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.103.48"); // Sunucu IP adresi

    // Baðlantýyý kurma
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server\n");

    // Kullanýcý adý isteði
    printf("Enter your name: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Yeni satýr karakterini kaldýr

    // Kullanýcý adýný sunucuya gönder
    send(sock, username, strlen(username), 0);

    // Mesajlarý almak için iþ parçacýðý baþlat
    pthread_create(&recv_thread, NULL, receive_messages, &sock);

    while (1) {
        // Kullanýcýdan mesaj al
        printf("[You] Enter your message: ");
        fflush(stdout); // Terminali temizlemeden önce göster

        fgets(buffer, BUFFER_SIZE, stdin);

        // Zaman damgasý ekle
        get_current_time(time_buffer, sizeof(time_buffer));
        snprintf(message_with_time, sizeof(message_with_time), "[%s - %s] %s", time_buffer, username, buffer);

        // Mesajý sunucuya gönder
        send(sock, message_with_time, strlen(message_with_time), 0);

        // Mesajlar arasýnda boþluk býrak
        printf("\n");
    }

    // Soketi kapat
    closesocket(sock);
    WSACleanup();

    return 0;
}

