/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "PJ_RPI.h"

void controlStateChange(int, char);

char state[27] = {0};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void setHttpHeader(char httpHeader[])
{
    // File object to return
    FILE *htmlData = fopen("index.html", "r");

    char line[100];
    char responseData[8000];
    while (fgets(line, 100, htmlData) != 0)
    {
        strcat(responseData, line);
    }
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    strcat(httpHeader, responseData);
}

void controlStateChange(int port, char statetemp)
{
    long input = GPIO_READ(port);

    if (input >> port ^ statetemp)
    {
        state[port] = !statetemp;
        printf("port: %i state: %i\n", port, state[port]);

        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";

    if (map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Here is the message: %s\n", buffer);

        int pin;
        sscanf(buffer, "%d", &pin);
        INP_GPIO(pin);
        OUT_GPIO(pin);

        for (size_t i = 0; i < 3; i++)
        {
            // Toggle (blink a led!)
            GPIO_SET = 1 << pin;
            printf("gpio%d-status: aan \n", pin);
            sleep(2);

            GPIO_CLR = 1 << pin;
            printf("gpio%d-status: uit \n", pin);
            sleep(2);
        }

        n = write(newsockfd, "toggled your led", 18);
        if (n < 0)
            error("ERROR writing to socket");

        send(newsockfd, httpHeader, sizeof(httpHeader), 0);
    }
    //close(newsockfd);
    close(sockfd);
}