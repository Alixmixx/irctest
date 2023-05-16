import os
from random import choices
import socket
from string import ascii_uppercase

MAX_CLIENTS = 3
SERVER_ADDRESS = 'localhost', 5555

for client_num in range(MAX_CLIENTS):
    pid = os.fork()
    if pid == 0:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(SERVER_ADDRESS)
        port = sock.getsockname()[1]
        message = "".join(choices(ascii_uppercase, k=10))
        sock.sendall(f"{port}: {message}".encode())
        sock.close()
        os._exit(0)