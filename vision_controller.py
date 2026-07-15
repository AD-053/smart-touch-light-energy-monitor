import cv2
import mediapipe as mp
import serial
import time

# -------------------------------------------------------------
# CONFIGURATION: Change 'COM3' to match your Arduino port!
# -------------------------------------------------------------
ARDUINO_PORT = '/dev/cu.usbmodem1301' 
BAUD_RATE = 9600

try:
    arduino = serial.Serial(port=ARDUINO_PORT, baudrate=BAUD_RATE, timeout=.1)
    print(f"--> Successfully connected to Arduino on {ARDUINO_PORT}")
    time.sleep(2) # Give Arduino time to auto-reset upon connection
except Exception as e:
    print(f"Error connecting to Arduino: {e}")
    print("Please double check your ARDUINO_PORT variable.")
    exit()

# Initialize MediaPipe Hand Tracking
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.8, min_tracking_confidence=0.8)
mp_draw = mp.solutions.drawing_utils

# Open the system webcam
cap = cv2.VideoCapture(0)
last_action = None

print("--> Camera initialized. Show all 5 fingers to turn ON, close your fist to turn OFF.")
print("--> Press 'q' inside the video window to quit.")

while cap.isOpened():
    success, img = cap.read()
    if not success:
        print("Failed to read from webcam.")
        break

    # Flip image horizontally for a natural mirror effect
    img = cv2.flip(img, 1)
    h, w, c = img.shape
    
    # Convert image colors to RGB for MediaPipe processing
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)
    
    fingers_open = 0
    status_text = "No Hand Detected"

    if results.multi_hand_landmarks:
        for hand_lms in results.multi_hand_landmarks:
            landmarks = hand_lms.landmark
            
            # Tip IDs and corresponding PIP joint IDs for the 4 standard fingers
            finger_tips = [8, 12, 16, 20]
            finger_pips = [6, 10, 14, 18]
            
            # Check 4 standard fingers: If tip is higher up (lower Y value) than PIP joint
            for i in range(4):
                if landmarks[finger_tips[i]].y < landmarks[finger_pips[i]].y:
                    fingers_open += 1
                    
            # Check Thumb: Compare tip X coordinate with IP joint X coordinate
            # (Works reliably since the image is flipped horizontally)
            if landmarks[4].x > landmarks[3].x:
                fingers_open += 1

            status_text = f"Fingers Open: {fingers_open}"

            # -------------------------------------------------------------
            # COMMAND ASSIGNMENT LOGIC
            # -------------------------------------------------------------
            # 5 Fingers Open -> Send command to turn ON / Reset countdown
            if fingers_open == 5:
                if last_action != "ON":
                    arduino.write(b"VISION_ON\n")
                    print("Sent Command: VISION_ON")
                    last_action = "ON"
            
            # 0 Fingers Open (Fist) -> Send command to turn OFF
            elif fingers_open == 0:
                if last_action != "OFF":
                    arduino.write(b"VISION_OFF\n")
                    print("Sent Command: VISION_OFF")
                    last_action = "OFF"
            
            # Draw visual tracking nodes on the screen
            mp_draw.draw_landmarks(img, hand_lms, mp_hands.HAND_CONNECTIONS)

    # Render data HUD text on the live video frame
    cv2.putText(img, status_text, (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.imshow("Smart Light Vision Engine", img)

    # Listen for incoming log updates from the Arduino and print them to the terminal
    if arduino.in_waiting > 0:
        try:
            arduino_msg = arduino.readline().decode('utf-8').strip()
            if arduino_msg:
                print(f"[Arduino Data]: {arduino_msg}")
        except:
            pass

    # Safe escape route by pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Clean up resource allocations
cap.release()
cv2.destroyAllWindows()
arduino.close()
print("--> System safely stopped.")