import signal
import socket
import sys
from termcolor import colored

BACKLOG = 8
BUFFER_SIZE = 1024

def forward_data(source, destination, color):
    data = source.recv(BUFFER_SIZE)
    if data:
        print(colored(data.decode(), color))
        destination.sendall(data)

def proxy_server():
    server_host = "localhost"
    proxy_host = "localhost"
    server_port = 6668
    proxy_port = 9999

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((proxy_host, proxy_port))
    server_socket.listen(BACKLOG)

    print(colored(f"Proxy server listening on port {proxy_port}...", "blue"))

    while True:
        client_socket, client_address = server_socket.accept()
        print(colored(f"Accepted connection from {client_address[0]}:{client_address[1]}", "blue"))
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.connect((server_host, server_port))
        while True:
            forward_data(client_socket, server_socket, "green")
            forward_data(server_socket, client_socket, "red")
        client_socket.close()
        server_socket.close()

def handle_sigint(sig, frame):
    print(colored("Good bye.", "blue"))
    sys.exit(0)

signal.signal(signal.SIGINT, handle_sigint)
proxy_server()
