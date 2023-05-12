package main

import (
	"fmt"
	"net"
	"os"
	"time"
)

func main() {
	c, _ := net.Dial("tcp", "127.0.0.1:7777")
	for i := 0; i < 10; i++ {
		c.Write([]byte(fmt.Sprintf("TESING %d", os.Getpid())))
		time.Sleep(time.Millisecond * time.Duration(10))
	}
	c.Close()
}