from dataclasses import dataclass
import re
import select
import socket
import sys
from termcolor import colored

BACKLOG = 32
BUFFER_SIZE = 1024


@dataclass
class Connection:
    idx: int
    client_socket: socket
    server_socket: socket


def print_info(message):
    print(colored(message, "blue"), end="\n")


def parse_address(s):
    host, port = s.split(":")
    return host, int(port)


def find_connection(connections, socket):
    for connection in connections:
        if socket == connection.client_socket or socket == connection.server_socket:
            return connection


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
proxy_socket.listen(BACKLOG)

total_connections = 0
connections = []
while True:
    sockets = [
        proxy_socket,
        *[s for c in connections for s in [c.client_socket, c.server_socket]],
    ]
    responses = select.select(sockets, [], [])[0]
    for sender in responses:
        if sender == proxy_socket:
            client_socket, _ = proxy_socket.accept()
            server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_socket.connect(SERVER_ADDRESS)
            total_connections += 1
            connection = Connection(total_connections, client_socket, server_socket)
            connections.append(connection)
            print_info(f"Client#{total_connections} connected.")
        else:
            data = sender.recv(BUFFER_SIZE)
            connection = find_connection(connections, sender)
            if sender == connection.client_socket:
                direction = f"Client#{connection.idx} to Server:"
                color = "red"
                receiver = connection.server_socket
            else:
                direction = f"Server to Client#{connection.idx}:"
                color = "green"
                receiver = connection.client_socket
            if not data:
                print_info(f"Client#{connection.idx} disconnected.")
                sender.close()
                receiver.close()
                connections.remove(connection)
            else:
                print(colored(direction, color))
                print(colored(data.decode().strip(), color))
                receiver.sendall(data)
