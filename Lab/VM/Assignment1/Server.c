#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
void crc();

// length of the generator polynomial
#define N strlen(gen_poly)
// data to be transmitted and received
char* data;
// CRC value
char check_value[28];
// generator polynomial
char* gen_poly;
// variables
int data_length, i, j;
// function that performs XOR operation
void XOR(){
    // if both bits are the same, the output is 0
    // if the bits are different the output is 1
    for (j = 1; j < N; j++)
        check_value[j] = ((check_value[j] == gen_poly[j]) ? '0' : '1');
}
// Function to check for errors on the receiver side
void receiver(){
    // get the received data
    printf("Enter the received data: ");
    scanf("%s", data);
    printf("\n-----------------------------\n");
    printf("Data received: %s", data);
    // Cyclic Redundancy Check
    crc();
    // Check if the remainder is zero to find the error
    for (i = 0; (i < N - 1) && (check_value[i] != '1'); i++)
        ;
    if (i < N - 1)
        printf("\nError detected\n\n");
    else
        printf("\nNo error detected\n\n");
}

void crc(){
    // initializing check_value
    for (i = 0; i < N; i++)
        check_value[i] = data[i];
    do
    {
        // check if the first bit is 1 and calls XOR function
        if (check_value[0] == '1')
            XOR();
        // Move the bits by 1 position for the next computation
        for (j = 0; j < N - 1; j++)
            check_value[j] = check_value[j + 1];
        // appending a bit from data
        check_value[j] = data[i++];
    } while (i <= data_length + N - 1);
    // loop until the data ends
}

int main(int argc, char *argv[]){
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server,
             sizeof(server)) < 0)
    {
        // print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    // accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client,
                         (socklen_t *)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    char *Msg1 = "Enter data";
    write(client_sock, Msg1, strlen(Msg1));
    // Receive a message from client
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
    {
        data = client_message;
        data_length = strlen(data);
        // appending n-1 zeros to the data
        for (i = data_length; i < data_length + N - 1; i++)
            data[i] = '0';
        // Send the message back to client
        write(client_sock, data, strlen(data));
        // print the data with padded zeros
        printf("\n Final padded data : %s", data);
    }
    char *Msg2 = "Enter key";
    write(client_sock, Msg2, strlen(Msg2));
    // Receive a message from client
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
    {
        gen_poly = client_message;
        crc();
        // Send the message back to client
        // write(client_sock , client_message , strlen(client_message));
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}