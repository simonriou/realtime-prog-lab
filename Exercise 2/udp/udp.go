package main

import (
"fmt"
"log"
"net"
"sync"
"time"
)

var port = ":20018"
var targetAddr = "10.100.23.204:20018"

func initUDP(addr string) *net.UDPAddr {
udpSendAddr, err := net.ResolveUDPAddr("udp", addr)
if err != nil {
log.Fatalf("Error resolving the UDP address: %v", err)
}

return udpSendAddr
}

func listenUDP(udpListenAddr *net.UDPAddr, wg *sync.WaitGroup) {
defer wg.Done()

conn, err := net.ListenUDP("udp", udpListenAddr)
if err != nil {
log.Fatalf("Error starting UDP listener: %v", err)
}

defer conn.Close()
fmt.Printf("Listening for messages on %s\n", port)

buffer := make([]byte, 1024)

for {
n, addr, err := conn.ReadFromUDP(buffer)
if err != nil {
log.Printf("Error reading UDP mesag10.100.23.204e: %v", err)
continue
}

fmt.Printf("Received message from %s: %s\n", addr.String(), string(buffer[:n]))
}
}

func sendUDP(udpSendAddr *net.UDPAddr) {
conn, err := net.DialUDP("udp", nil, udpSendAddr)
if err != nil {
log.Fatalf("Error starting UDP: %v", err)
}

defer conn.Close()
message := "Hello Server."

_, err = conn.Write([]byte(message))
if err != nil {
log.Fatalf("Error sending UDP message: %v", err)
}

fmt.Printf("Message sent to %s: %s\n", targetAddr, message)
}

func main() {
udpListenAddr := initUDP(port)
udpSendAddr := initUDP(targetAddr)

var wg sync.WaitGroup
wg.Add(1)

go listenUDP(udpListenAddr, &wg)
time.Sleep(time.Second)
go sendUDP(udpSendAddr)

wg.Wait()
}