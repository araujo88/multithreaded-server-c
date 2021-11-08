// HTTP/1.1 GET request by hostname/IPv4 address
// Usage:
// By hostname:
// ./client.o -hostname www.example.com -port 80
// By IP-address (IPv4):
// ./client.o -ipaddr 74.6.231.21 -port 80
// The last one sends a GET request for www.yahoo.com
// In both examples the default port for internet traffic is utilized

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // close socket
#include <errno.h>  // error codes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h> // gethostbyname / gethostbyaddr

#define BUFFER_SIZE 4096
#define PORT 9002

int main(int argc, char *argv[])
{
    char ip_address[] = "127.0.0.1";
    int network_socket; // create a socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    puts("Creating socket ...");
    if (network_socket < 0)
    {
        perror("Socket creation error");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }
    puts("Socket created!");

    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;                    // specifies protocol IPv4
    server_address.sin_port = htons(PORT);                  // specifies port (host to network)
    server_address.sin_addr.s_addr = inet_addr(ip_address); // converts IP string to standard IPv4 decimal notation

    // connection
    printf("Connecting to server %s...\n", ip_address);
    if ((connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address))) < 0)
    {
        perror("Connection error");
        printf("Error code: %d\n", errno);
        return EXIT_FAILURE;
    }
    puts("Connected!");

    struct hostent *he = gethostbyaddr(&server_address.sin_addr.s_addr, sizeof(server_address.sin_addr.s_addr), AF_INET);
    char *hostname = he->h_name;

    struct in_addr server_addr;
    char server_ip_addr[INET_ADDRSTRLEN];
    char server_ip_addr6[INET6_ADDRSTRLEN];

    if (he)
    {
        printf("name: %s\n", he->h_name);
        while (*he->h_aliases)
        {
            printf("alias: %s\n", *he->h_aliases++);
        }
        while (*he->h_addr_list)
        {
            bcopy(*he->h_addr_list++, (char *)&server_addr, sizeof(server_addr));
            inet_ntop(AF_INET, &server_addr, server_ip_addr, INET_ADDRSTRLEN);
            inet_ntop(AF_INET6, &server_addr, server_ip_addr6, INET6_ADDRSTRLEN);
            printf("IPv4 address: %s\n", server_ip_addr);
            printf("IPv6 address: %s\n", server_ip_addr6);
        }
    }
    else
    {
        printf("error");
    }

    // sends message to the server
    char request[BUFFER_SIZE] = "GET / HTTP/1.1\r\n"; // GET request for root page
    char header2[128] = "Host:";
    char header3[128] = "Connection: close";
    char port[4];
    strcat(header2, hostname);
    strcat(header2, ":");
    sprintf(port, "%d\r\n", PORT);
    strcat(header2, port);
    strcat(header3, "\r\n\r\n");
    strcat(request, header2);
    strcat(request, header3);
    printf("Request:\n%s\n", request);
    puts("Sending request ...");
    send(network_socket, request, strlen(request), 0);

    // Receive data from the server
    char buffer[BUFFER_SIZE]; // server response
    int buffer_length;

    if (recv(network_socket, &buffer, sizeof(buffer), 0))
    {
        if (buffer == NULL)
        {
            printf("\n** Error getting response **\n");
        }
        else
        {
            printf("---------------------------------------------------------------\n");
            printf("\n\nResponse:\n%s\n", buffer);
            memset(buffer, 0, sizeof(buffer));
        }
    }

    // close the socket
    close(network_socket);
    return 0;
}