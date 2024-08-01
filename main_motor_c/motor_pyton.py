import cv2
import mediapipe as mp
import serial
import time
import math

# Initialize MediaPipe hand module
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

# Initialize serial communication with Arduino
arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)  # Update 'COM3' to your Arduino's port

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline().decode('utf-8').strip()
    return data

def calculate_distance(p1, p2):
    return math.sqrt((p2.x - p1.x) ** 2 + (p2.y - p1.y) ** 2)

# Capture video from the default camera
cap = cv2.VideoCapture(0)
hands = mp_hands.Hands()

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(image)
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            
            # Control base and arm servos
            wrist = hand_landmarks.landmark[mp_hands.HandLandmark.WRIST]
            index_tip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            thumb_tip = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP]
            
            
            if wrist.x < 0.4:
                write_read('R')
            elif wrist.x > 0.6:
                write_read('L')
            if wrist.y < 0.4:
                write_read('U')
            elif wrist.y > 0.6:
                write_read('D')

            # Control claw servo
            distance = calculate_distance(index_tip, thumb_tip)
            if distance > 0.1:  # Adjust the threshold as needed
                write_read('C')  # Send 'O' for open hand
            else:
                write_read('O')  # Send 'C' for closed hand

    cv2.imshow('Handtracker', image)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
