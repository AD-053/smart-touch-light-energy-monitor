
---

# 💡 Project Title: *Smart Touch & Vision Controlled Light with Energy Monitoring*

A smart hybrid lighting system built using **Arduino UNO**, a physical **Touch Sensor (TTP223)**, and a **Computer Vision Controller**. The project provides seamless dual-control (physical touch and contactless hand gestures), automatic energy saving, continuous usage monitoring, energy consumption estimation, and real-time smart analytics through the Serial Monitor interface.

---

# 🎓 Course Information

| Item | Details |
| --- | --- |
| Course Title | Microprocessor and Interfacing Lab |
| Course ID | CSE 0714 3178 |
| Credits | 1.5 |
| Semester | 3rd Year, 1st Semester |

---

# 👨‍💻 Team Information

**Team Name:** `Group_05_Section_A`

| Name | Registration No. | Project Contribution |
| --- | --- | --- |
| **Md Shajjadul Ferdous** | 2022331015 | Wrote the math formulas for **Energy Consumption ($Wh$)** and calculated the **Electricity Cost** in Tk. |
| **Autanu Datta** | 2022331053 | Created the **Time Tracking Logic** and the **Analytics Engine**, including the 24-hour automatic report and `report`/`reset` commands. |
| **Taposh Kumer Ghosh** | 2022331089 | Built the Python script, added the **MediaPipe/OpenCV Hand Gesture Control** (`VISION_ON`/`VISION_OFF`), and fixed connection bugs. |
| **Rahad Islam** | 2022331097 | Designed the circuit layout, set up the **TTP223 Touch Sensor** with debouncing, and built the physical **Mini House Prototype**. |

---

# 📖 Project Introduction

The main goal of this project is to build a hybrid, intelligent light control system using an Arduino UNO, a touch sensor, and an integrated computer vision processor.

A user can turn the LED ON or OFF either by physically touching the touch sensor or by showing simple hand gestures (open hand or fist) to a connected webcam.

Unlike a standard automated light, this project intelligently saves electricity by turning the light OFF after a period of inactivity, warning the user before doing so, tracking complete session statistics, estimating power usage in Watt-hours, and dynamically calculating live monetary cost projections.

---

# 🛠 Hardware Used

| Component | Description | Estimated Price |
| --- | --- | --- |
| **Arduino UNO R3** | Main microcontroller board running the core firmware loop | 760 Tk |
| **TTP223 Touch Sensor** | Capacitive touch module providing physical interface control | 60 Tk |
| **LED** | Light-emitting diode acting as the smart light source | 2 Tk |
| **Breadboard** | Solderless prototyping board for circuit connections | 50 Tk |
| **Jumper Wires** | Interconnecting wires (Male-to-Male / Female-to-Male) | 50 Tk |
| **Mini House Setup** | Structural housing/enclosure built for the smart light prototype | 100 Tk |
| **Host PC / Laptop + Webcam** | Core system powering the OpenCV/MediaPipe tracking layer | *Existing Device* |
| **Total Hardware Cost** | *(Excluding Host PC/Laptop)* | **1022 Tk** |

(Note: The prices are estimated standard local market rates in Bangladeshi Taka (Tk) to match the currency context of your code's billing module).
---

# 💻 Software Used

* Arduino IDE
* C++ (Arduino Firmware)
* Python 3.x (Vision Framework Engine)
* OpenCV & MediaPipe (Hand Tracking Libraries)
* Wokwi Simulator

---

# 🔄 System Workflow

```text
       Power ON
          │
          ▼
    Arduino Starts
          │
          ▼
 Wait for Touch / Gesture
          │
    ┌─────┴─────────────────────┐
    ▼                           ▼
Touch Detected OR Gesture (All Fingers Open)
    │                           │
    └─────────────┬─────────────┘
                  ▼
               LED ON
                  │
                  ▼
        30 Second Timer Starts
                  │
                  ▼
              25 Seconds
                  │
                  ▼
          Warning Countdown
             5, 4, 3, 2, 1
                  │
       ┌──────────┴──────────┐
       │                     │
       ▼                     ▼
 Touch / Gesture Open    No Interaction
       │                     │
       ▼                     ▼
   Reset Timer           Sleep Mode
       │                     │
       └──────────────►  LED OFF

```

---

# 📂 Repository Structure

```text
.
├── README.md
├── SmartTouchLight.ino      # Main Arduino firmware supporting Touch & Serial Vision overrides
├── vision_controller.py     # Python OpenCV/MediaPipe tracking script
├── sketch.ino               # Wokwi simulation source
├── diagram.json             # Wokwi board config
├── circuit_wokwi.png        # Physical schematic visualizer
└── wokwi-project.txt        # Simulator reference notes

```

---

# ▶️ How to Run

## 1. Setting Up the Firmware (Arduino IDE)

1. Connect your Arduino UNO to your PC using a USB cable.
2. Open `SmartTouchLight.ino` inside the Arduino IDE.
3. Select your core board type (**Arduino UNO**) and identify your active serial port (e.g., `/dev/cu.usbmodem1301` or `COM3`).
4. Upload the code to your board.
5. **Important:** Close the Arduino IDE Serial Monitor window so the port is free for the Vision Controller.

## 2. Setting Up the Vision Engine (Python)

1. Open your terminal or command prompt and install the structural core dependencies:
```bash
pip3 install opencv-python mediapipe==0.10.14 pyserial

```


2. Open `vision_controller.py` and ensure the `ARDUINO_PORT` variable matches your active port exactly.
3. Run the vision tracking layer:
```bash
python3 vision_controller.py

```


4. A window showing your camera feed will appear. Hold up **5 open fingers** to turn the light ON, and close your hand into a **fist (0 fingers)** to turn it OFF.

---

# 🔌 Circuit Connections

| Component | Arduino Pin | Description |
| --- | --- | --- |
| Touch Sensor OUT | D7 | Digital input pin reading human touch states |
| LED (+) | D13 | Output driver pin (Arduino built-in status indicator) |
| LED (-) | GND | Ground loop passing through a 220Ω protection resistor |
| Touch Sensor VCC | 5V | Constant system power line |
| Touch Sensor GND | GND | Shared system ground plane |

---

# 📸 Project Preview

## Wokwi Circuit

<p align="center">
<img src="circuit_wokwi.png" width="700">
</p>

---

# 🔗 Wokwi Simulation

You can run the touch-controlled firmware portion of this project online without any hardware components.

**Simulation Link**

[https://wokwi.com/projects/469553316004649985](https://wokwi.com/projects/469553316004649985)

---

# ✨ Features

## 1. Touch-Controlled Light

* Touch once → LED turns **ON**
* Touch again → LED turns **OFF**

This replaces traditional electrical toggle switches with a responsive physical touch module.

---

## 2. Contactless Vision Control (New)

* **All 5 Fingers Open** → Sends `VISION_ON` → LED turns **ON**
* **All Fingers Closed (Fist)** → Sends `VISION_OFF` → LED turns **OFF**

Integrates smart automation by processing camera data frames using Google MediaPipe and transmitting localized Serial overrides via USB execution loops.

---

## 3. Auto OFF (Energy Saving)

When the LED turns ON (via Touch or Vision), an internal **30-second timer** begins counting down. If no subsequent touch or open-hand gesture is registered within 30 seconds, the LED switches OFF automatically.

---

## 4. Smart Reminder

After **25 seconds** of run time, the system prints out alerts across the communication channel to indicate an impending automated shutdown:

```text
WARNING!
Auto OFF in 5 seconds...
5
4
3
2
1

```

---

## 5. Sleep Mode

During the last 5 seconds of the session window:

* The LED blinks slowly as a visual cue.
* Tapping the touch sensor or presenting **5 open fingers** to the camera halts the countdown, resets the base clock, and returns the LED to a solid ON state.
* If left completely unattended, the light turns OFF.

---

## 6. Touch Counter

Tracks physical interactions over the lifetime of a monitoring run.
Example:

```text
Touch Count : 1
Touch Count : 2
Touch Count : 3

```

---

## 7. Total ON Time

Logs active run times to ensure precise telemetry records:
Example:

```text
Current Session
35 seconds
-------------------
Total ON Time
7 minutes 35 seconds

```

---

## 8. Energy Consumption

Calculates dynamic electrical work consumption values.
Formula:


$$\text{Energy (Wh)} = \text{Power (Watts)} \times \left(\frac{\text{Time (Seconds)}}{3600}\right)$$


Example output display:

```text
Energy Used
0.005 Wh

```

---

## 9. Estimated Electricity Cost

Computes financial calculations based on raw power consumption metrics using the system cost variables.
Example output display:

```text
Estimated Cost
0.00006 Tk

```

---

## 10. Auto OFF Counter

Tracks total energy-saving interventions triggered by the device software.
Example:

```text
Auto OFF Events
8

```

---

## 11. Smart Light Analytics

An encapsulated analytics engine storing active run data profiles including total touch counts, overall run times, average session lengths, calculated power consumption, financial tracking costs, and automated safety events.

---

# 🖥 Serial Monitor Commands

The internal firmware parses command phrases sent manually from the console or programmatically by your scripts:

## `report`

Displays an explicit snapshot breakdown of the monitoring run without resetting current tracking tables.
Example:

```text
========================================
SMART LIGHT ANALYTICS
========================================

Current Monitoring Session

Touch Count          : 18
Total ON Time        : 12m 25s
Energy Used          : 0.008 Wh
Estimated Cost       : 0.00010 Tk
Auto OFF Events      : 6
Average Session Time : 41 sec

========================================

```

---

## `reset`

Pushes the analytics table out to the terminal log before executing a hard clear of internal variables to initialize a brand-new telemetry block.
Example:

```text
========================================
SMART LIGHT ANALYTICS
========================================

Monitoring Session Ended

Touch Count          : 48
Total ON Time        : 1h 12m 45s
Energy Used          : 0.050 Wh
Estimated Cost       : 0.00060 Tk
Auto OFF Events      : 15
Average Session Time : 1m 30s

========================================

Manual Reset Requested...

Statistics Cleared Successfully.

New Monitoring Session Started.

```

---

## `VISION_ON` / `VISION_OFF`

Automated inputs handled directly by the computer vision backend loop script to transition electrical states or reset sleep warnings via the USB serial bus.

---

# ⏰ Automatic 24-Hour Report

After running continuously for **24 hours**, the system automatically compiles, displays, and flushes data states to keep analytics records safe from variable overflow constraints.

```text
========================================
SMART LIGHT ANALYTICS
========================================

24-Hour Monitoring Completed

Touch Count          : 87
Total ON Time        : 3h 28m 15s
Energy Used          : 0.144 Wh
Estimated Cost       : 0.00173 Tk
Auto OFF Events      : 27
Average Session Time : 2m 24s

========================================

24-Hour Session Completed.

Statistics Reset Automatically.

New Monitoring Session Started.

```

---

# 📊 Statistics Collected

The system records:

* Total Physical Touch Count
* Total LED ON Time Accumulation
* Calculated Average Session Time Length
* Precise Energy Consumption Computation ($Wh$)
* Local Unit Electricity Price Conversions ($Tk$)
* Automated Safe Shutdown Event Totals

---

# ⚡ Energy Calculation

Formula:


$$\text{Energy} = \text{Power} \times \text{Time}$$

* **LED Base Load Power:** `0.04 Watt`
* **LED Continuous Run Time Example:** `2 Minutes`
* **Resultant Energy Calculation:** `0.00133 Wh`
* **Resultant Financial Projection:** `0.000016 Tk`

---

# 🎯 Applications

* Smart Home Integration
* Motion/Gesture Controlled Spaces (Hostel, Classroom, Offices)
* Automated Energy Auditing Systems
* Hands-Free Sanitary Control Interfaces
* Embedded Systems & Real-Time Data Analysis Demonstrations

---

# 🌟 Project Innovation

This project transitions from a basic hardware toggle to a smart **hybrid edge controller**.

Key innovations include:

* **Dual Layer Interaction:** Physical touch sensing and computer vision systems work concurrently.
* **Embedded Telemetry:** Performs time-tracking, data reduction, and physics-based financial conversions directly on-chip.
* **Proactive Interventions:** Provides automated reminders, slow blinking warning profiles, and gesture override extensions to prevent unexpected blackouts.
* **Automated Lifecycle Processing:** Implements command-driven and interval-driven scheduling models for hands-free management.

---

# 📝 Conclusion

The **Smart Touch & Vision Controlled Light with Energy Monitoring** application provides a functional, highly integrated framework showing how standard digital inputs can merge with desktop computer vision systems.

Key architectural patterns showcased include:

* Concurrent Digital I/O Handling
* Asynchronous, Non-Blocking Serial Parsing
* State Machine Processing Frameworks
* Low-Overhead Floating Point Mathematical Evaluations
* Computer Vision Feature Tracking Pipelines

---

# 📜 License

This project was developed for the **Microprocessor and Interfacing Lab (CSE 0714)** course for academic and educational purposes.