import errno
import os
import re
import signal
import socket
import sys
from termcolor import colored

BUFFER_SIZE = 1024
BACKLOG = 128

def is_valid_address(s):
    return re.fullmatch(r".*:\d{4,5}", s)

def parse_address(s):
    host, port = s.split(":")
    return host, int(port)

if len(sys.argv) != 3 or not re.fullmatch(r".*:\d{4,5}", sys.argv[1]) or not re.fullmatch(r"\d{4,5}", sys.argv[2]):
    print(f"Usage: python3 {sys.argv[0]} server_host:server_port proxy_port")
    print(f"Example: python3 {sys.argv[0]} localhost:6669 5555")
    sys.exit(1)
SERVER_ADDRESS = SERVER_HOST, SERVER_PORT = parse_address(sys.argv[1])
PROXY_ADDRESS = PROXY_HOST, PROXY_PORT = "localhost", int(sys.argv[2])

def print_info(message):
    print(colored(message, "blue"), end="\n\n")

def tryclose(sock):
    try:
        sock.close()
    except:
        print_info(f"Could not close socket {sock}")

def parent_sigint(sig, frame):
    print
    print_info("\rGood bye.")
    tryclose(server_socket)
    tryclose(proxy_socket)
    sys.exit(0)

def child_sigint(sig, frame):
    tryclose(server_socket)
    tryclose(client_socket)
    sys.exit(0)

def grim_reaper(signum, frame):
    while True:
        try:
            pid, status = os.waitpid(-1, os.WNOHANG)
        except OSError:
            return
        if pid == 0:
            return

def parent_signals():
    signal.signal(signal.SIGINT, parent_sigint)
    signal.signal(signal.SIGCHLD, grim_reaper)

def child_signals():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGCHLD, signal.SIG_DFL)

def forward_data(sender, receiver, color):
    data = sender.recv(BUFFER_SIZE)
    if data:
        print(colored(f"{sender.getsockname()[1]} to {receiver.getsockname()[1]}:", color))
        print(colored(data.decode(), color))
        receiver.sendall(data)

server_socket = proxy_socket = client_socket = None
parent_signals()

proxy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
proxy_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
proxy_socket.bind(PROXY_ADDRESS)
proxy_socket.listen(BACKLOG)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.connect(SERVER_ADDRESS)

print_info(f"Proxy listening on port {PROXY_PORT}, talking to {SERVER_PORT}.")
while True:
    client_socket, client_address = proxy_socket.accept()
    print_info(f"Client :{client_address[1]} connected.")
    pid = os.fork()
    if pid == 0:
        child_signals()
        proxy_socket.close()
        if os.fork() == 0:
            while True:
                forward_data(client_socket, server_socket, "green")
        elif os.fork() == 0:
            while True:
                forward_data(server_socket, client_socket, "red")
        else:
            os.waitpid(-1, os.WNOHANG)
            os.waitpid(-1, os.WNOHANG)
    else:
        client_socket.close()