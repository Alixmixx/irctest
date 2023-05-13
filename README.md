# Manual testing

1. 1st terminal : `make && valgrind --track-origins=yes ./ircserv 6667`
2. 2nd terminal : `go run scripts/client.go`
3. 2nd terminal : `go run scripts/client.go & go run scripts/client.go & go run scripts/client.go`
4. 1st terminal : Ctrl+C or `quit`
