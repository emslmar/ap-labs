package main

import (
	"bufio"
	"fmt"
	"log"
	"math/rand"
	"os"
	"os/exec"
	"sort"
	"strconv"
	"sync"
	"time"
)

type Carro struct {
	id              int
	tiepoActual     float64
	velocidadActual int
	velocidadMax    int
	posicion        int
	vueltas         int
	terminoCarrera  bool
	posX            int
	posY            int
}

var pilotos []Carro
var tablero [5][90]int
var wg sync.WaitGroup
var startTime time.Time
var mu sync.RWMutex
var vueltas int
var numPilotos int

func main() { //Emiliano

	c := exec.Command("clear")
	c.Stdout = os.Stdout
	c.Run()

	numPilotos, vueltas = obtenerCarrosVueltas()
	hilos := make(chan Carro, numPilotos)
	agregarPilotos(numPilotos)
	iniciarHilos(hilos)

	var ganadores []int
	close(hilos)
	for carroActual := range hilos {
		ganadores = append(ganadores, carroActual.id)
	}

	imprimeResultados(ganadores)

}

func obtenerCarrosVueltas() (int, int) { //Emiliano

	fmt.Printf("F1 Competition!!! Welcome!!!\n")
	fmt.Print("\nEnter number of racers (3-9): ")
	strCarros := scanner()
	fmt.Print("\nPlease enter the number of vueltas (2-10): ")

	strVueltas := scanner()

	numCarros, err := strconv.Atoi(strCarros)

	if numCarros < 3 || numCarros > 10 {
		log.Fatal("Invalid number of numCarros")
	}

	vueltas, err := strconv.Atoi(strVueltas)

	if vueltas < 2 || vueltas > 15 {
		log.Fatal("Invalid number of vueltas")
	}

	if err != nil {
		log.Fatal(err, " Please enter integers")
	}

	return numCarros, vueltas
}

func agregarPilotos(numPilotos int) { //Paulina
	var fila, columna int = -1, 0
	min := 6
	max := 12
	for i := 0; i < numPilotos; i++ {
		rand.Seed(time.Now().UnixNano())

		if (i % len(tablero)) == 0 {
			fila++
			columna = 0
		}
		var newCarro Carro
		newCarro.velocidadActual = 0
		newCarro.posY = columna
		newCarro.id = i + 1
		newCarro.vueltas = 0
		newCarro.posX = fila
		newCarro.terminoCarrera = false
		newCarro.velocidadMax = rand.Intn(max-min) + min
		newCarro.posicion = 0

		tablero[columna][fila] = newCarro.id
		pilotos = append(pilotos, newCarro)
		columna++
	}
}

func iniciarHilos(hilos chan Carro) { //Paulina
	startTime = time.Now()
	for !gameOver(hilos) {
		for i := 0; i < len(pilotos); i++ {
			wg.Add(1)
			go moverCarros(&pilotos[i], hilos)
		}
		time.Sleep(time.Second * 1)

		wg.Wait()
		tiepoActual := time.Since(startTime)
		for i := 0; i < len(pilotos); i++ {
			if pilotos[i].vueltas < vueltas {
				pilotos[i].tiepoActual = tiepoActual.Seconds()
			}
		}
		imprimirPista()
	}
}

func moverCarros(carroActual *Carro, hilos chan Carro) { //Emiliano
	defer wg.Done()

	if !carroActual.terminoCarrera {
		posicionActualX := carroActual.posX
		posicionActualY := carroActual.posY
		nuevaY := carroActual.posY
		carroActual.velocidadActual = calcularAceleracion(carroActual.velocidadActual, carroActual.velocidadMax)
		nuevaX := posicionActualX + carroActual.velocidadActual

		mu.Lock()
		nuevaX, nuevaY = manejarCollision(posicionActualX, posicionActualY, nuevaX)
		carroActual.velocidadActual = nuevaX - carroActual.posX

		mu.Unlock()
		if nuevaX > (len(tablero[0]) - 1) {
			if carroActual.vueltas < vueltas {
				carroActual.vueltas++
			}
			nuevaX = nuevaX % (len(tablero[0]) - 1)
			if carroActual.vueltas == vueltas {
				carroActual.terminoCarrera = true
				if carroActual.terminoCarrera {
					mu.Lock()
					tablero[carroActual.posY][carroActual.posX] = 0
					mu.Unlock()
					carroActual.posX = -1
					carroActual.posY = -1
					carroActual.posicion = (carroActual.vueltas * len(tablero[0]))
					hilos <- *carroActual
					return

				}

			}
		}
		mu.Lock()
		tablero[posicionActualY][posicionActualX] = 0
		tablero[nuevaY][nuevaX] = carroActual.id
		mu.Unlock()

		carroActual.posX = nuevaX
		carroActual.posY = nuevaY

		carroActual.posicion = (carroActual.vueltas * len(tablero[0])) + nuevaX

	}

}

func calcularAceleracion(velocidadActual int, velocidadMax int) int { //Paulina
	if velocidadActual >= 0 && velocidadActual < velocidadMax-2 {
		return velocidadActual + 1
	}
	min := velocidadMax - 2
	max := velocidadMax
	rand.Seed(time.Now().UnixNano())
	return (rand.Intn(max-min) + min)

}

func manejarCollision(posicionActualX int, posicionActualY int, nuevaX int) (int, int) { //Emiliano
	nuevaY := posicionActualY
	for i := posicionActualX; i < nuevaX; i++ {

		if tablero[posicionActualY][(i+1)%(len(tablero[0])-1)] != 0 {

			return i, cambiarPista(i%(len(tablero[0])-1), posicionActualY)
		}
	}
	return nuevaX, nuevaY

}

func cambiarPista(posicionActualX int, posicionActualY int) int { //Paulina
	if posicionActualY > 0 && posicionActualY < (len(tablero)-1) {
		if tablero[posicionActualY+1][posicionActualX] == 0 {
			return (posicionActualY + 1)
		} else if tablero[posicionActualY-1][posicionActualX] == 0 {
			return (posicionActualY - 1)
		}
	} else if posicionActualY == 0 {
		if tablero[posicionActualY+1][posicionActualX] == 0 {
			return (posicionActualY + 1)
		}
	} else if posicionActualY == (len((tablero)) - 1) {
		if tablero[posicionActualY-1][posicionActualX] == 0 {
			return (posicionActualY - 1)
		}
	}
	return posicionActualY
}

func imprimirPista() { //Emiliano
	c := exec.Command("clear")
	c.Stdout = os.Stdout
	c.Run()
	fmt.Printf("	           							 | POSITION | CAR |  LAPS  | SPEED  |   TIME    |\n")
	calcularPos()

	for i := 0; i < len(tablero); i++ {
		fmt.Printf("| START | ")
		for j := 0; j < len(tablero[i]); j++ {
			if tablero[i][j] == 0 {
				fmt.Printf("- ")
			} else {
				fmt.Printf("%d ", tablero[i][j])
			}
		}
		fmt.Printf(" | FINISH |")
		fmt.Printf("\n")
	}

}

func imprimeResultados(ganadores []int) { //Emiliano
	fmt.Println("\n")
	fmt.Println("\n")
	fmt.Println("		           							 ++++++++++++++ FIRST PLACE ++++++++++++++++++")
	fmt.Printf("           								                   	   Driver %d\n", ganadores[0])
	fmt.Println("		           							 ++++++++++++++ SECOND PLACE ++++++++++++++++++")
	fmt.Printf("          	 							                   	   Driver %d\n", ganadores[1])
	fmt.Println("		           							 ++++++++++++++ THIRD PLACE ++++++++++++++++++")
	fmt.Printf("           								                   	   Driver %d\n", ganadores[2])

}

func calcularPos() { //Paulina -- calcula si va en primer segundo tercer lugar etc.
	var posId []float64
	actualTime := time.Since(startTime).Seconds()
	for i := 0; i < len(pilotos); i++ {
		if pilotos[i].terminoCarrera {
			posId = append(posId, float64(pilotos[i].posicion)*(actualTime-pilotos[i].tiepoActual))
		} else {
			posId = append(posId, float64(pilotos[i].posicion))
		}

	}
	sort.Float64s(posId)
	var posActual []int
	for i := 0; i < len(posId); i++ {
		for j := 0; j < len(pilotos); j++ {

			if pilotos[j].terminoCarrera {
				if posId[i] == (float64(pilotos[j].posicion) * (actualTime - pilotos[j].tiepoActual)) {
					posActual = append(posActual, j)
				}
			} else {
				if posId[i] == (float64(pilotos[j].posicion)) {
					posActual = append(posActual, j)
				}
			}

		}
	}
	posActual = quitarExtra(posActual)
	var contadorPos int = 1
	for i := len(pilotos) - 1; i >= 0; i-- {

		speed := pilotos[posActual[i]].velocidadActual
		playerID := posActual[i]
		vueltaCarro := pilotos[posActual[i]].vueltas
		time := pilotos[posActual[i]].tiepoActual

		if (vueltaCarro + 1) > vueltas {
			vueltaCarro = vueltas
		} else {
			vueltaCarro++
		}
		fmt.Printf(" 	  	        						 |    %d°    |  %d  |   %d/%d  | %d0Km/h | %f |\n", contadorPos, playerID+1, vueltaCarro, vueltas, speed, time)
		contadorPos++

	}

}

func scanner() string {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	return scanner.Text()

}

func gameOver(hilos chan Carro) bool {
	maxLaps := 0
	for i := 0; i < len(pilotos); i++ {
		if pilotos[i].vueltas == vueltas {
			maxLaps++
		}
	}
	if maxLaps == len(pilotos) {
		return true
	}
	return false

}

func quitarExtra(intSlice []int) []int {
	llaves := make(map[int]bool)
	lista := []int{}
	for _, entrada := range intSlice {
		if _, valor := llaves[entrada]; !valor {
			llaves[entrada] = true
			lista = append(lista, entrada)
		}
	}
	return lista
}
