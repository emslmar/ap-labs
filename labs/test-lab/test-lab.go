package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) > 1 {
		fmt.Print("Hello ")
		for i := 1; i < len(os.Args); i++ {
			fmt.Print(os.Args[i])
			fmt.Print(" ")
		}
		fmt.Print("Welcome to the Jungle \n")
	} else {
		fmt.Println("You are missing the parameter required : name")
	}

}
