import socket
import select



######################DO ZROBIENIA#########################
#1. wysylanie spowrotem do klientow TAK ZEBY DZIALALO











HEADER_LENGTH = 1024

PORT = 1234

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

hostname = socket.gethostname()

ip_hostname = socket.gethostbyname(hostname)

server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_socket.bind((ip_hostname, PORT))

server_socket.listen()

sockets_list = [server_socket]

clients = {}

print("Listening for connections on {}:{}...".format(ip_hostname, PORT))


def receive_message(client_socket):

    try:

        inc_message = client_socket.recv(HEADER_LENGTH)

        if not len(inc_message):
            return False


        return {'data': inc_message.decode('utf-8').strip()}

    except:
        return False

while True:

    read_sockets, _, exception_sockets = select.select(sockets_list, [], sockets_list)

    for notified_socket in read_sockets:

        if notified_socket == server_socket:

            client_socket, client_address = server_socket.accept()

            print("Connected: {}".format(client_address))

            user = receive_message(client_socket)

            if user is False:
                print("ERRORUSER")
                continue

            sockets_list.append(client_socket)

            clients[client_socket] = user

            print('Accepted new connection from {}, username: {}'.format(client_address, user["data"]))

        else:

            message = receive_message(notified_socket)

            if message is False:
                print('{} disconnected'.format(clients[notified_socket]))
                sockets_list.remove(notified_socket)
                del clients[notified_socket]
                continue

            user = clients[notified_socket]

            print('Wiadomosc od {}: {}'.format(user["data"], message["data"]))

            for client_socket in clients:

                if client_socket != notified_socket:
                    #client_socket.send(user['header'] + user["data"] + message['header'] + message['data'])
                    continue

                #print("KONIEC")
    for notified_socket in exception_sockets: #mozna potem usunac

        sockets_list.remove(notified_socket)
s
        del clients[notified_socket]