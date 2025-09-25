
import socket
import json

def main():
    HOST = '127.0.0.1'
    PORT = 9999

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    print(f"Attempting to connect to {HOST}:{PORT}...")
    
    try:
        client_socket.connect((HOST, PORT))
        print("Successfully connected to the server.")
        
        buffer = ""
        while True:
            try:
                data = client_socket.recv(1024).decode('utf-8')
                if not data:
                    print("Server closed the connection.")
                    break
                
                buffer += data
                
                # Process all complete JSON objects in the buffer
                while '\n' in buffer:
                    message, buffer = buffer.split('\n', 1)
                    try:
                        landmark_data = json.loads(message)
                        print("-------------------- Received Landmark Data --------------------")
                        # Pretty print the JSON data
                        print(json.dumps(landmark_data, indent=2))
                    except json.JSONDecodeError:
                        print(f"Received malformed JSON: {message}")

            except socket.error as e:
                print(f"Socket error during receive: {e}")
                break

    except ConnectionRefusedError:
        print("Connection failed. Is the server running?")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        print("Closing client socket.")
        client_socket.close()

if __name__ == '__main__':
    main()
