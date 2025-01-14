import socket

# Server config
HOST = '127.0.0.1' # Localhost
PORT = 12345 # Port to listen on

# Create UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the address and port
server_socket.bind((HOST, PORT))

print(f"UDP server up and listening on {HOST}:{PORT}")

# Listen for incoming UDP messages
try:
    while True:
        data, client_address = server_socket.recvfrom(1024) # Buffer size is 1024 bytes
        print(f"Received data from {client_address}: {data.decode()}")

        # Send a response back to the client
        response = "Hello from the server! Message received: " + data.decode()
        server_socket.sendto(response.encode(), client_address)
except KeyboardInterrupt:
    print("Server shutting down...")
finally:
    server_socket.close()