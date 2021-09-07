// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 { return p.x }

func (p Point) Y() float64 { return p.y }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), (q.Y() - p.Y()))
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	res := math.Hypot(q.X()-p.X(), q.Y()-p.Y())
	fmt.Printf("%.2f ", res)
	return res
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := 1; i < len(path); i++ {
		sum += path[i-1].Distance(path[i])
		fmt.Printf("+ ")

	}
	sum += path[len(path)-1].Distance(path[0])
	fmt.Printf("= ")
	return sum
}

// Given three colinear points p, q, r, the function checks if
//point q lies on line segment 'pr'
func onSegment(p Point, q Point, r Point) bool {
	if q.x <= math.Max(p.X(), r.X()) && q.X() >= math.Min(p.X(), r.X()) && q.Y() <= math.Max(p.Y(), r.Y()) && q.Y() >= math.Min(p.Y(), r.Y()) {
		return true
	}
	return false
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
func doIntersect(p1 Point, q1 Point, p2 Point, q2 Point) bool {

	var o1 int = orientation(p1, q1, p2)
	var o2 int = orientation(p1, q1, q2)
	var o3 int = orientation(p2, q2, p1)
	var o4 int = orientation(p2, q2, q1)

	// General case
	if o1 != o2 && o3 != o4 {
		return true
	}

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	}

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1
	if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	}

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}

	return false // Doesn't fall in any of the above cases
}

func orientation(p Point, q Point, r Point) int {
	res := (q.Y()-p.Y())*(r.X()-q.X()) -
		(q.X()-p.X())*(r.Y()-q.Y())

	if res == 0 {
		return 0
	} // colinear

	if res > 0 {
		return 1
	} //clockwise
	return 2 //counterclockwise
}

func generatePoints(pointsArr []Point, n int) {
	for i := 0; i < n; i++ {
		pointsArr[i] = Point{-100 + rand.Float64()*(100+100), -100 + rand.Float64()*(100+100)}
	}
}

func main() {
	flag := true
	var sides int

	if len(os.Args) <= 1 {
		fmt.Printf("No se recibio el numero de lados\n")
		flag = false
	} else if len(os.Args) > 2 {
		fmt.Printf("Se recibieron demasiados parametros\n")
		flag = false
	} else {
		num, err := strconv.Atoi(os.Args[1])
		if err == nil {
			if num >= 3 {
				sides = num
				fmt.Printf(" - Generating a [%d] sides figure\n", sides)
			} else {
				flag = false
				fmt.Printf("Tiene que tener mas de dos lados para ser una figura geometrica \n")
			}

		} else {
			fmt.Print("Se ingreso un valor incorrecto donde corresponde el numero de puntos\n")
			flag = false
		}
	}

	if flag {
		//generate n sides or paths
		var pointsArr []Point = make([]Point, sides)
		generatePoints(pointsArr, sides)
		pointsArr[0] = Point{-100 + rand.Float64()*(100+100), -100 + rand.Float64()*(100+100)}
		pointsArr[1] = Point{-100 + rand.Float64()*(100+100), -100 + rand.Float64()*(100+100)}

		for i := 2; i < sides; i++ {
			pointsArr[i] = Point{-100 + rand.Float64()*(100+100), -100 + rand.Float64()*(100+100)}
			//check if it intersects with any previous
			for j := 0; j < i-2; j++ { //nunca va a chocar con el anterior
				if doIntersect(pointsArr[j], pointsArr[j+1], pointsArr[i-1], pointsArr[i]) {
					i = i - 1
					break
				}
				if i == sides-1 {
					if j == 0 {
						continue
					} else if doIntersect(pointsArr[j], pointsArr[j+1], pointsArr[i], pointsArr[0]) {
						i = i - 1
						break
					}
				}
			}

		}
		//falta verificar el ultimo

		fmt.Printf(" - Figure's vertices\n")
		for i := 0; i < sides; i++ {
			fmt.Printf("   - (%6.1f, %6.1f)\n", pointsArr[i].X(), pointsArr[i].Y())
		}

		fmt.Printf(" - Figure's Perimeter\n")
		var camino Path = pointsArr
		fmt.Printf("   - ")
		fmt.Printf("%f \n", camino.Distance())

		//pointsArr.Distance()
		//check points dont intersect with each other
	}
}

//!-path
