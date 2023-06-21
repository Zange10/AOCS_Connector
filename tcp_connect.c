#include "tcp_connect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.0.100"  // Server IP address
#define SERVER_PORT 1234           // Port number

int client_socket;
struct sockaddr_in server_address;
char *message = "Hello from the client!";
char buffer[1024];

void init_socket() {
    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr));

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return ;
    }
}

void close_socket() {
    // Close the connection
    close(client_socket);
}

int get_from_tcp() {
    // Send data to the server
    send(client_socket, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // Receive a response from the server
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Received response from server: %s\n", buffer);

    return 0;
}
