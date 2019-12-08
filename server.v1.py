import socket
import select



###################### DO ZROBIENIA ###############################

#1. wysylanie duzego pliku (min 15MB)
#2. pozmieniac nazwy zmiennych, funkcji itp i lekko poprzestawiac
# strukture zeby nie bylo widac ze kradzony kod XD
#3. ewentualny debugging, testowanie itp. Po prostu potestuj i sprawdz czy nie ma jakichs bledow ktorych nie wykrylem

###################################################################

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

            user = receive_message(client_socket)

            if user is False:
                print("errorUser")
                continue

            sockets_list.append(client_socket)

            clients[client_socket] = user

            print('New connection from {}, username: {}'.format(client_address, user["data"]))

        else:

            message = receive_message(notified_socket)

            if message is False:
                str = clients[notified_socket]
                print('{} disconnected'.format(str['data']))
                sockets_list.remove(notified_socket)
                del clients[notified_socket]
                continue

            user = clients[notified_socket]
            if message["data"] == "\n" or message["data"] == "" or message["data"] == " ":
                for client_socket in clients:
                    if client_socket == notified_socket:
                        client_socket.send("\0".encode('utf-8'))
            else:
                print('Message from {}: {}'.format(user["data"], message["data"]))

                for client_socket in clients:

                    if client_socket != notified_socket:
                        answer = '{} > {}\n'.format(user["data"], message['data'])
                        client_socket.send(answer.encode('utf-8'))
                        continue
                    elif client_socket == notified_socket:
                        client_socket.send("\0".encode('utf-8'))
