# UDP Test Server

This is a simple UDP server implemented in Python. It listens for incoming UDP messages on a specified host and port, and sends a response back to the client.

## Server Configuration

- **Host:** `127.0.0.1` (Localhost)
- **Port:** `12345`

## How to Run the Server

1. Ensure you have Python installed on your system.
2. Save the server script (`server.py`) to your desired directory.
3. Open a terminal and navigate to the directory containing `server.py`.
4. Run the server using the following command:
    ```sh
    python server.py
    ```
5. The server will start and listen for incoming UDP messages.

## How to Test the Server

Make sure you keep your terminal window open to keep the server running.
You can use a UDP client to send messages to the server. Here is an example using Python:

```python
import socket

# Client config
HOST = '127.0.0.1'
PORT = 12345

# Create UDP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send a message to the server
message = "Hello, server!"
client_socket.sendto(message.encode(), (HOST, PORT))

# Receive a response from the server
response, server_address = client_socket.recvfrom(1024)
print(f"Received response from server: {response.decode()}")

# Close the socket
client_socket.close()
```

## Handling Server Shutdown

To gracefully shut down the server, you can use `Ctrl+C` in the terminal where the server is running. This will trigger a `KeyboardInterrupt` and close the server socket.

```python
try:
    # ...existing code...
except KeyboardInterrupt:
    print("Server shutting down...")
finally:
    server_socket.close()
