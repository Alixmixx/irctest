import socket
import sys
from _thread import start_new_thread

RESET = "\u001b[0m"
RED = "\u001b[31m"
GREEN = "\u001b[32m"
BLUE = "\u001b[34m"

PROXY_PORT = 5556
PROXY_HOST = ""
SERVER_PORT = 6668
SERVER_HOST = ""

BACKLOG = 8
BUFFER_SIZE = 512

def proxy_server(conn_server, addr_server, data):
    print("c->s |", data)
    should_exit = False
    try:
        sock_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock_client.connect((SERVER_HOST, SERVER_PORT))
        sock_client.send(data)
        while True:
            reply = sock_client.recv(BUFFER_SIZE)
            print("s->c |", reply)
            if len(reply) > 0:
                conn_server.send(reply)
            else:
                break
    except Exception as e:
        print(f"{RED}proxy_server, {e}{RESET}")
        should_exit = True
    sock_client.close()
    conn_server.close()
    if should_exit:
        sys.exit(1)

try:
    sock_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock_server.bind((PROXY_HOST, PROXY_PORT))
    sock_server.listen(BACKLOG)
    print(f"{GREEN}Proxy listening on port {PROXY_PORT}{RESET}")
except Exception as e:
    print(f"{RED}Unable to Initialize Socket{RESET}")
    sock_server.close()
    sys.exit(1)
while True:
    try:
        conn_server, addr_server = sock_server.accept()
        data = b""
        while True:
            buf = conn_server.recv(BUFFER_SIZE)
            if not buf:
                break
            data += buf
        start_new_thread(proxy_server, (conn_server, addr_server, data))
    except KeyboardInterrupt:
        sock_server.close()
        print(f"{GREEN}\rGood bye.{RESET}")
        sys.exit(0)
    except Exception as e:
        sock_server.close()
        print(f"{RED}\rGood bye.\n{e}{RESET}")
        sys.exit(1)