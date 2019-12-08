#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define PORT 1234

int main(void)
{
    char rec_mes[BUFFER_SIZE];
    char HOST[128];
    struct sockaddr_in adr;
    struct hostent *he;
    int received, client_socket;

    printf("Provide server's hostname.\n");
    fflush(stdout);
    fgets(HOST, 128, stdin);
    strtok(HOST, " \n\t\v\r");
    he = gethostbyname(HOST);
        if (he == NULL)
        {
            printf("Unknown host\n");
            return 0;
        }

    client_socket = socket(PF_INET, SOCK_STREAM, 0);

    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr = *(struct in_addr*) he->h_addr;

    if(connect(client_socket, (struct sockaddr*) &adr, sizeof(adr)) < 0)
    {
        printf("Cannot connect.\n");
        return 1;
    }
    printf("Connection established.\n");

    printf("Provide username: ");

    fflush(stdout);

    char username[BUFFER_SIZE];

    fgets(username, (BUFFER_SIZE + 1), stdin);

    strtok(username, "\n");

    send(client_socket, username, strlen(username), 0);

    char wiadomosc[BUFFER_SIZE], dest[BUFFER_SIZE];
    while(1)
    {
        printf("%s: ", username);
        fflush(stdout);
        fgets(wiadomosc, BUFFER_SIZE, stdin);

            char temp[BUFFER_SIZE], delim[] = " \n";
            strcpy(dest, wiadomosc);
            strtok(dest, delim);

            if(strcmp(dest, "/stop") == 0 || strcmp(dest, "/exit") == 0 || strcmp(dest, "/disconnect") == 0 || strcmp(dest, "/wyjdz") == 0 || strcmp(dest, "/rozlacz") == 0 || strcmp(dest, "/quit") == 0)
            {
                printf("Disconnected\n");
                break;
            }
            send(client_socket, wiadomosc, strlen(wiadomosc), 0);
            memset(rec_mes, '\0', BUFFER_SIZE);
            while(1)
            {
                if(!recv(client_socket, rec_mes, BUFFER_SIZE, 0))
                {
                    printf("Server has closed the connection\n");
                    exit(1);
                }
                else if(/*strlen(rec_mes) == 0 || */strcmp(rec_mes, "\0") == 0/* || rec_mes == 0*/) break;
                else
                {
                    printf("%s", rec_mes);
                }
            }
    }
    close(client_socket);

    return 0;
}
