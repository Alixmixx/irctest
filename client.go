package main

import (
	"fmt"
	"net"
	"os"
	"time"
)

func main() {
	msg := fmt.Sprintf("TESTING")
	msgPrefix := "some-prefix-to-prevent-arbitrary-connection"
	
	c, _ := net.Dial("tcp", "127.0.0.1:7777")
	for i := 0; i < 10; i++ {
		c.Write([]byte(fmt.Sprintf("%s%s %d", msgPrefix, msg, os.Getpid())))
		time.Sleep(time.Millisecond * time.Duration(10))
	}
	c.Write([]byte("This won't be parsed"))
	c.Close()
}