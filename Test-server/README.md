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
You can use a UDP client to send messages to the server. Using the `send.c` file from the `exercise-2` branch.

## Handling Server Shutdown

To gracefully shut down the server, you can use `Ctrl+C` in the terminal where the server is running. This will trigger a `KeyboardInterrupt` and close the server socket.

```python
try:
    # ...existing code...
except KeyboardInterrupt:
    print("Server shutting down...")
finally:
    server_socket.close()
