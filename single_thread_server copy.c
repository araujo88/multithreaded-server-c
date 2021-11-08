#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // close socket
#include <errno.h>   // error codes
#include <stdbool.h> // boolean types
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <signal.h> // for interrupt signal handler

#define PORT 9002
#define BUFFER_SIZE 1024

int server_socket; // global variable in order to be handled after SIGINT

void handle_signal(int sig);                                                               // interrupt signal handler
void check_socket(int server_socket);                                                      // check socket creation
void check_bind(int server_socket, struct sockaddr_in *server_address);                    // check binding
void check_listen(int server_socket, int number_connections);                              // check connection listening
void check_accept(int server_socket, int *client_socket, struct sockaddr *client_address); // check accepting connection
void send_HTML(int client_socket);                                                         // sends HTML data

int main(int argc, char *argv[])
{
    puts("Creating socket ...");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    check_socket(server_socket);
    puts("Socket created!");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    puts("Binding socket ...");
    check_bind(server_socket, &server_address);
    puts("Binding done!");

    signal(SIGINT, handle_signal);
    while (true)
    {
        puts("Waiting for incoming requests... (press Ctrl+C to quit)\n");
        check_listen(server_socket, 5);

        int client_socket;
        struct sockaddr_in *client_address = NULL;

        check_accept(server_socket, &client_socket, (struct sockaddr *)client_address);
        puts("Request accepted!\n");

        puts("Sending message ...\n");
        send_HTML(client_socket);
        puts("Message send!\n");
    }
    return 0;
}

void handle_signal(int sig)
{
    printf("\nCaught interrupt signal %d\n", sig);
    // closes the socket
    puts("Closing socket ...");
    if (close(server_socket) == 0)
    {
        puts("Socket closed!");
        exit(0);
    }
    else
    {
        perror("An error occurred while closing the socket: ");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

void check_socket(int server_socket)
{
    if (server_socket < 0)
    {
        perror("Socket failed: ");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

void check_bind(int server_socket, struct sockaddr_in *server_address)
{
    if ((bind(server_socket, (struct sockaddr *)server_address, sizeof(*server_address))) < 0)
    {
        perror("Bind failed");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

void check_listen(int server_socket, int num_connections)
{
    if ((listen(server_socket, num_connections)) < 0)
    {
        perror("Listen failed");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

void check_accept(int server_socket, int *client_socket, struct sockaddr *client_address)
{
    if ((*client_socket = accept(server_socket, (struct sockaddr *)client_address, (socklen_t *)sizeof(client_address))) < 0)
    {
        perror("Accept failed");
        printf("Error code: %d\n", errno);
        exit(1);
    }
}

void send_HTML(int client_socket)
{
    char server_message[BUFFER_SIZE];
    char *current_date;
    char *content;
    time_t t; // define response date
    time(&t);

    current_date = ctime(&t);
    current_date[strcspn(current_date, "\n")] = 0; // removes newline for correct parsing

    content = "Hello from the server!"; // define response content (HTML)
    sprintf(server_message, "HTTP/1.0 200 OK\nDate: %s\nContent-Type: text/html\nContent-Length: %ld\n\n%s", current_date, strlen(content), content);

    send(client_socket, &server_message, sizeof(server_message), 0); // sends the message
    memset(server_message, 0, sizeof(server_message));               // sets server data to null pointer (cleanup)
}