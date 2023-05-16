import errno
import os
import signal
import socket
import sys
from termcolor import colored

SERVER_ADDRESS = SERVER_HOST, SERVER_PORT = 'localhost', 6668
PROXY_ADDRESS = PROXY_HOST, PROXY_PORT = 'localhost', 5555
BUFFER_SIZE = 1024
BACKLOG = 128

def set_signals():
    def handle_sigint(sig, frame):
        print(colored("\rGood bye.", "blue"))
        sys.exit(0)

    signal.signal(signal.SIGINT, handle_sigint)

    def grim_reaper(signum, frame):
        while True:
            try:
                pid, status = os.waitpid(-1, os.WNOHANG)
            except OSError:
                return
            if pid == 0:
                return

    signal.signal(signal.SIGCHLD, grim_reaper)

def print_info(message):
    print(colored(message, "blue"))

def get_socket(address):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(address)
    sock.listen(BACKLOG)
    return sock

def forward_data(sock, destination, color):
    data = sock.recv(BUFFER_SIZE)
    if data:
        print(colored(data.decode(), color))
        destination.sendall(data)

set_signals()
proxy_socket = get_socket(PROXY_ADDRESS)
server_socket = get_socket(SERVER_ADDRESS)
print_info(f"Proxy listening on port {PROXY_PORT}, talking to {SERVER_PORT}.")
while True:
    client_socket, client_address = proxy_socket.accept()
    print_info(f"Client :{client_address[1]} connected.")
    pid = os.fork()
    if pid == 0:
        proxy_socket.close()
        request = client_socket.recv(BUFFER_SIZE)
        print(request.decode())
        http_response = b"oui"
        client_socket.sendall(http_response)
        client_socket.close()
        os._exit(0)
    else:
        client_socket.close()