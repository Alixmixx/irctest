import re
import select
import socket
import sys
from termcolor import colored

BUFFER_SIZE = 1024


def print_info(message):
    print(colored(message, "blue"), end="\n")


def forward_data(receiver, data, direction, color):
    if not data:
        print_info(direction + " empty message.")
        sys.exit(0)
    print(colored(direction, color))
    print(colored(data.decode().strip(), color))
    receiver.sendall(data)


def parse_address(s):
    host, port = s.split(":")
    return host, int(port)


if (
    len(sys.argv) != 3
    or not re.fullmatch(r"\d{4,5}", sys.argv[1])
    or not re.fullmatch(r".*:\d{4,5}", sys.argv[2])
):
    print(f"Usage: python3 {sys.argv[0]} proxy_port server_host:server_port")
    print(f"Example: python3 {sys.argv[0]} 5555 188.240.145.40:6667")
    sys.exit(1)
PROXY_ADDRESS = _, PROXY_PORT = "127.0.0.1", int(sys.argv[1])
SERVER_ADDRESS = SERVER_HOST, SERVER_PORT = parse_address(sys.argv[2])
print_info(f"Listening on port {PROXY_PORT}.")
print_info(f"Forwarding to {sys.argv[2]}.")

proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
proxy_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
proxy_socket.bind(PROXY_ADDRESS)
proxy_socket.listen(1)
client_socket, _ = proxy_socket.accept()

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.connect(SERVER_ADDRESS)

sockets = [client_socket, server_socket]
while True:
    s_read, _, _ = select.select(sockets, [], [])
    for s in s_read:
        data = s.recv(BUFFER_SIZE)
        if s == client_socket:
            forward_data(server_socket, data, "Client to Server:", "red")
        elif s == server_socket:
            forward_data(client_socket, data, "Server to Client:", "green")
        else:
            assert False
