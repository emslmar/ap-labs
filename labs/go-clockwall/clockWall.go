package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"strings"
	"sync"
)

func main() {
	var allClocks sync.WaitGroup
	for i := 1; i < len(os.Args); i++ {
		splitTimezone := strings.Split(os.Args[i], "=")
		c, err := net.Dial("tcp", splitTimezone[1])
		println(splitTimezone[0])
		println(splitTimezone[1])
		if err != nil {
			fmt.Println(err)
		}
		allClocks.Add(1)
		go printTimezone(c, &allClocks, splitTimezone[0])

	}
	allClocks.Wait()
}

func printTimezone(c net.Conn, allClocks *sync.WaitGroup, city string) {
	defer allClocks.Done()
	for {
		_, err := io.WriteString(c, city)
		_, err = io.Copy(os.Stdout, c)
		if err != nil {
			return 
		}

	}
}