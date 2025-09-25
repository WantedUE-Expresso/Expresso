
import cv2
import mediapipe as mp
import socket
import json
import argparse
import numpy as np

def draw_landmarks(image, hand_landmarks, desired_indices):
    """Draws only the desired landmarks on the image."""
    annotated_image = image.copy()
    
    # Define drawing specs
    circle_radius = 5
    circle_color = (0, 255, 0) # Green
    
    h, w, _ = annotated_image.shape
    for idx, landmark in enumerate(hand_landmarks.landmark):
        if idx in desired_indices:
            # Convert normalized coordinates to pixel coordinates
            cx, cy = int(landmark.x * w), int(landmark.y * h)
            cv2.circle(annotated_image, (cx, cy), circle_radius, circle_color, -1) # -1 for filled circle
    return annotated_image

def main(visualize):
    # ----------------- Socket Setup -----------------
    HOST = '127.0.0.1'
    PORT = 9999
    
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(1)
    
    print(f"Server listening on {HOST}:{PORT}. Waiting for a client to connect...")
    
    client_socket, addr = server_socket.accept()
    print(f"Client connected from {addr}")

    # ----------------- MediaPipe Hands Setup -----------------
    mp_hands = mp.solutions.hands
    hands = mp_hands.Hands(
        max_num_hands=2,
        min_detection_confidence=0.7,
        min_tracking_confidence=0.7)

    # Define desired landmark indices for filtering and drawing
    desired_landmark_indices = [
        mp_hands.HandLandmark.THUMB_TIP,
        mp_hands.HandLandmark.INDEX_FINGER_TIP
    ]

    # ----------------- Webcam Setup -----------------
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open webcam.")
        client_socket.close()
        server_socket.close()
        return

    try:
        while cap.isOpened():
            success, image = cap.read()
            if not success:
                print("Ignoring empty camera frame.")
                continue

            # To improve performance, optionally mark the image as not writeable to
            # pass by reference.
            image.flags.writeable = False
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            results = hands.process(image)

            # Prepare image for drawing
            image.flags.writeable = True
            image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
            
            landmark_data = []
            if results.multi_hand_landmarks:
                for hand_landmarks in results.multi_hand_landmarks:
                    # Extract landmark data for Wrist, Thumb, and Index Finger (indices 0-8)
                    landmarks = []
                    
                    for idx, landmark in enumerate(hand_landmarks.landmark):
                        if idx in desired_landmark_indices:
                            landmarks.append({
                                'id': idx,
                                'x': landmark.x,
                                'y': landmark.y
                            })
                    landmark_data.append(landmarks)

                    # Draw landmarks if visualization is enabled
                    if visualize:
                        image = draw_landmarks(image, hand_landmarks, desired_landmark_indices)

            # Send data to client
            if landmark_data:
                try:
                    json_data = json.dumps(landmark_data)
                    client_socket.sendall(json_data.encode('utf-8') + b'\n') # Use newline as a delimiter
                except (socket.error, BrokenPipeError) as e:
                    print(f"Client disconnected: {e}")
                    break # Exit loop if client disconnects

            # Show visualization if enabled
            if visualize:
                try:
                    cv2.imshow('MediaPipe Hands', cv2.flip(image, 1))
                    if cv2.waitKey(5) & 0xFF == 27: # Press ESC to exit
                        break
                except cv2.error as e:
                    print(f"OpenCV GUI Error: {e}")
                    print("Visualization might not be supported in this environment. Disabling visualization.")
                    visualize = False # Disable visualization to continue processing data
                except Exception as e:
                    print(f"An unexpected error occurred during visualization: {e}")
                    visualize = False # Disable visualization
    
    finally:
        # ----------------- Cleanup -----------------
        print("Shutting down server and cleaning up resources.")
        hands.close()
        cap.release()
        client_socket.close()
        server_socket.close()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='MediaPipe Hand Tracking Socket Server')
    parser.add_argument('--visualize', action='store_true', help='Enable live visualization of the hand tracking.')
    args = parser.parse_args()
    
    main(args.visualize)
