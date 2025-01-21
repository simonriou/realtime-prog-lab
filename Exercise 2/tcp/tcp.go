package main

import (
	"bufio"
	"fmt"
	"net"
	"strings"
	"sync"
	"time"
)

const (
	serverAddress = "10.100.23.204:33546" // Server address
)

func main() {
	// Connect to the server
	conn, err := net.Dial("tcp", serverAddress)
	if err != nil {
		fmt.Println("Error connecting to server:", err)
		return
	}
	defer conn.Close()
	fmt.Println("Connected to server:", serverAddress)

	var wg sync.WaitGroup

	// Start a goroutine to continuously receive messages
	wg.Add(1)
	go func() {
		defer wg.Done()
		receiveMessages(conn)
	}()

	// Start a goroutine to send messages
	go func() {
		sendMessages(conn)
	}()

	wg.Wait() // Wait for all goroutines to finish
}

// Function to receive messages from the server
func receiveMessages(conn net.Conn) {
	reader := bufio.NewReader(conn)
	for {
		// Read the server's response until '\0'
		message, err := reader.ReadString('\x00')
		if err != nil {
			fmt.Println("Error reading from server:", err)
			return
		}
		// Print the received message (without the null character)
		fmt.Println("Server:", strings.TrimSuffix(message, "\x00"))
	}
}

// Function to send messages to the server
func sendMessages(conn net.Conn) {
	message := "Hello Server."

	// Trim newline and add null character
	message = strings.TrimSpace(message) + "\x00"

	// Send the message to the server
	_, err := conn.Write([]byte(message))
	if err != nil {
		fmt.Println("Error sending message:", err)
		return
	}

	time.Sleep(time.Second)

	_, err = conn.Write([]byte(message))
	if err != nil {
		fmt.Println("Error sending message:", err)
		return
	}
}