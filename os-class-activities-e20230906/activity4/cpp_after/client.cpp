#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

#define PORT 9002
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0]
             << " <server_ip> <student_name>" << endl;
        return 1;
    }

    const char *server_ip = argv[1];
    const char *student_name = argv[2];

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip,
                  &server_address.sin_addr) <= 0)
    {
        perror("Invalid server IP");
        close(client_socket);
        return 1;
    }

    if (connect(client_socket,
                (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0)
    {
        perror("Connection failed");
        close(client_socket);
        return 1;
    }

    cout << "Connected to server!" << endl;

    send(client_socket, student_name,
         strlen(student_name), 0);

    char buffer[BUFFER_SIZE] = {0};

    int bytes_received = recv(client_socket, buffer,
                              BUFFER_SIZE - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        cout << "Server response: " << buffer << endl;
    }
    else
    {
        cout << "No response from server." << endl;
    }

    close(client_socket);
    return 0;
}
