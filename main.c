//import socket
//import select
//import errno

//lol include wszystko
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>


//HEADER_LENGTH = 10
/*int HEADER_LENGTH = 1024;*/

//IP = "150.254.79.22"
char IP[] = "150.254.79.110";
//char IP[] = "127.0.0.1";



//PORT = 1234
int PORT = 1234;
//char HOST[] = "pc24e";

int main(void)
{
    struct sockaddr_in adr;

    struct hostent *he;

    char HOST[128];
    printf("Podaj hostname servera.\n");
    fflush(stdout);
    fgets(HOST, 129, stdin);
    strtok(HOST, " \n\t\v\r");

    he = gethostbyname(HOST); //polaczenie po hoscie

    if (he == NULL) {
            printf("Nieznany host\n");
            return 0;
        }

    int client_socket = socket(PF_INET, SOCK_STREAM, 0); //PRZYPISANIE SOCKETU

     adr.sin_family = AF_INET;
     adr.sin_port = htons(PORT);
     adr.sin_addr = *(struct in_addr*) he->h_addr; //przy polaczeniu po hoscie
     //adr.sin_addr.s_addr = inet_addr(IP);

    if(connect(client_socket, (struct sockaddr*) &adr, sizeof(adr)) < 0) //POLACZENIE NR1
    {
        printf("Polaczenie nie powiodlo sie\n");
        return 1;
    }
    printf("Polaczenie nawiazane.\n");

    printf("Podaj nazwe uzytkownika:\n");

    fflush(stdout); //fgetc(stdin);

    char username[1024];
    fgets(username, 1025, stdin);

    printf("Username: %s\n", username);

    strtok(username, "\n");

    printf("Username: %s\n", username);

    send(client_socket, username, strlen(username), 0);

    //fgetc(stdin);

    char wiadomosc[1024], dest[1024];
    while(1)
    {

        printf("Podaj wiadomosc: \n");
        fflush(stdout);
         fgets(wiadomosc, 1025, stdin);
         printf("TEST: %s\n", wiadomosc);
         /*if(strlen(wiadomosc) == 0)
         {
             printf("DUPA\n");
             continue;
         }
        else
        {
            char *temp, delim[] = " \n\t\v\r";
         strcpy(dest, wiadomosc);
        temp = strtok(dest, delim);

         if(strcmp(temp, "/stop") == 0)
            {
                printf("TAK\n");
                break;
            }*/
            //printf("BFRSEND\n");
            send(client_socket, wiadomosc, strlen(wiadomosc), 0);
            printf("Wiadomosc wyslana.\n");
            //printf("KONIEC\n");
        //}

    }

    /*

    //my_username = input("Username: ")
    char my_username[128];
    printf("%s", "Username: ");
    gets(my_username);



    //client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


    //client_socket.connect((IP, PORT))

    printf("Polaczono");
    */






    close(client_socket);

    return 0;
}

/*

# Set connection to non-blocking state, so .recv() call won;t block, just return some exception we'll handle
client_socket.setblocking(False)

# Prepare username and header and send them
# We need to encode username to bytes, then count number of bytes and prepare header of fixed size, that we encode to bytes as well
username = my_username.encode('utf-8')
username_header = f"{len(username):<{HEADER_LENGTH}}".encode('utf-8')
client_socket.send(username_header + username)

while True:

    # Wait for user to input a message
    message = input(f'{my_username} > ')

    # If message is not empty - send it
    if message:

        # Encode message to bytes, prepare header and convert to bytes, like for username above, then send
        message = message.encode('utf-8')
        message_header = f"{len(message):<{HEADER_LENGTH}}".encode('utf-8')
        client_socket.send(message_header + message)

    try:
        # Now we want to loop over received messages (there might be more than one) and print them
        while True:

            # Receive our "header" containing username length, it's size is defined and constant
            username_header = client_socket.recv(HEADER_LENGTH)

            # If we received no data, server gracefully closed a connection, for example using socket.close() or socket.shutdown(socket.SHUT_RDWR)
            if not len(username_header):
                print('Connection closed by the server')
                sys.exit()

            # Convert header to int value
            username_length = int(username_header.decode('utf-8').strip())

            # Receive and decode username
            username = client_socket.recv(username_length).decode('utf-8')

            # Now do the same for message (as we received username, we received whole message, there's no need to check if it has any length)
            message_header = client_socket.recv(HEADER_LENGTH)
            message_length = int(message_header.decode('utf-8').strip())
            message = client_socket.recv(message_length).decode('utf-8')

            # Print message
            print(f'{username} > {message}')

    except IOError as e:
        # This is normal on non blocking connections - when there are no incoming data error is going to be raised
        # Some operating systems will indicate that using AGAIN, and some using WOULDBLOCK error code
        # We are going to check for both - if one of them - that's expected, means no incoming data, continue as normal
        # If we got different error code - something happened
        if e.errno != errno.EAGAIN and e.errno != errno.EWOULDBLOCK:
            print('Reading error: {}'.format(str(e)))
            sys.exit()

        # We just did not receive anything
        continue

    except Exception as e:
        # Any other exception - something happened, exit
        print('Reading error: '.format(str(e)))
        sys.exit()
        */
