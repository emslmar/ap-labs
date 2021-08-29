package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	result := values[0]
	if operator == 1 {
		for i := 1; i < len(values); i++ {
			result += values[i]
		}
	} else if operator == 2 {
		for i := 1; i < len(values); i++ {
			result -= values[i]
		}
	} else if operator == 3 {
		for i := 1; i < len(values); i++ {
			result *= values[i]
		}
	}
	fmt.Printf("%d \n", result)
	return result
}

func main() {
	operator := 0
	flag := true
	if os.Args[1] == "add" {
		operator = 1
	} else if os.Args[1] == "sub" {
		operator = 2
	} else if os.Args[1] == "mult" {
		operator = 3
	} else {
		fmt.Printf("No se reconocio el operador, recuerda  que solo aceptamos las palabras sum, sub y mult\n")
		flag = false
	}

	nValues := len(os.Args) - 2
	arr := make([]int, nValues)
	pos := 0

	for i := 2; i < len(os.Args); i++ {
		num, err := strconv.Atoi(os.Args[i])
		if err == nil {
			arr[pos] = num
			pos++
		} else {
			fmt.Print("Se ingreso un valor incorrecto donde corresponde un numero\n")
			flag = false
		}

	}
	if flag {
		calc(operator, arr)
	}

}
