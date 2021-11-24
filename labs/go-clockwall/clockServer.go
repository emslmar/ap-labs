package main

import (
	"io"
	"net"
	"os"
	"time"
)

func connectionHandler(c net.Conn) {
	defer c.Close()
	timezone := os.Getenv("TZ")
	for {
		_, err := io.WriteString(c, timezone+time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	var port string
	if len(os.Args) != 3 {
		println("Not enought or to many arguments")
		return
	}
	if os.Args[1] == "-port" {
		port = "localhost: " + os.Args[2]
	} else {
		port = "localhost:9090"
	}
	listener, err := net.Listen("tcp", port)
	if err != nil {
		println(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			println(err) // e.g., connection aborted
			continue
		}
		go connectionHandler(conn) // handle connections concurrently
	}
}
