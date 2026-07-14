// Pin Configurations
const int touchPin = 7;
const int ledPin = 13;

// State Variables
bool ledState = false;
bool lastTouchState = LOW;
bool warningMode = false;
bool blinkState = true;
int countdown = 5;

// Timers (Using unsigned long to prevent overflow bugs)
unsigned long startTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastCountTime = 0;
unsigned long twentyFourHourTimer = 0;

// Analytics Counters
unsigned int touchCount = 0;
unsigned int autoOffCount = 0;
unsigned int sessionCount = 0;
unsigned long totalOnTimeMs = 0; // Cumulative time across completed sessions

// Energy Calculation Constants
const double LED_POWER_WATTS = 0.04;    // 0.04 Watts (given in documentation)
const double COST_PER_WH = 0.012;       // 12 Tk per kWh = 0.012 Tk per Wh

// Serial Input Buffer
String inputBuffer = "";

void setup() {
  pinMode(touchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  twentyFourHourTimer = millis(); // Initialize the 24-hour baseline

  Serial.println("========================================");
  Serial.println("  Smart Touch Light System Initialized  ");
  Serial.println("========================================");
}

void loop() {
  // 1. Handle Automatic 24-Hour Report
  // 86400000 milliseconds = 24 hours
  if (millis() - twentyFourHourTimer >= 86400000UL) {
    printReport("24-Hour Monitoring Completed");
    resetStatistics();
    Serial.println("24-Hour Session Completed.");
    Serial.println("Statistics Reset Automatically.");
    Serial.println("New Monitoring Session Started.");
  }

  // 2. Handle Serial Commands ("report" and "reset")
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      inputBuffer.trim();
      if (inputBuffer.equalsIgnoreCase("report")) {
        printReport("Current Monitoring Session");
      } 
      else if (inputBuffer.equalsIgnoreCase("reset")) {
        printReport("Monitoring Session Ended");
        Serial.println("Manual Reset Requested...");
        resetStatistics();
        Serial.println("Statistics Cleared Successfully.");
        Serial.println("New Monitoring Session Started.");
      }
      inputBuffer = ""; // Clear buffer after evaluation
    } else {
      inputBuffer += c;
    }
  }

  // 3. Handle Touch Sensor Inputs
  bool touchState = digitalRead(touchPin);

  if (touchState == HIGH && lastTouchState == LOW) {
    touchCount++;
    Serial.println();
    Serial.print("Touch Count : ");
    Serial.println(touchCount);

    if (!ledState) {
      // Feature 1: LED turns ON
      ledState = true;
      digitalWrite(ledPin, HIGH);
      startTime = millis();
      sessionCount++; // Increment session count for average time tracking

      warningMode = false;
      countdown = 5;
      blinkState = true;

      Serial.println("Light ON");
      Serial.println("Timer Started (30 sec)");
    } 
    else {
      if (warningMode) {
        // Feature 4: Touch during warning mode cancels countdown
        warningMode = false;
        countdown = 5;
        digitalWrite(ledPin, HIGH);
        startTime = millis();

        Serial.println("Touch Detected");
        Serial.println("Countdown Cancelled");
        Serial.println("Timer Reset (30 sec)");
        Serial.println("Light Remains ON");
      } 
      else {
        // Feature 1: Touch again while ON turns it OFF
        ledState = false;
        digitalWrite(ledPin, LOW);
        totalOnTimeMs += (millis() - startTime); // Save session duration
        Serial.println("Light OFF");
      }
    }
    delay(200); // Hardware debouncing
  }
  lastTouchState = touchState;

  // 4. Feature 3: Enter Warning Mode after 25 seconds of continuous run time
  if (ledState && !warningMode && (millis() - startTime >= 25000)) {
    warningMode = true;
    countdown = 5;
    lastBlinkTime = millis();
    lastCountTime = millis();

    Serial.println();
    Serial.println("----------------------------");
    Serial.println("WARNING!");
    Serial.println("Auto OFF in 5 seconds...");
    Serial.println(countdown);
  }

  // 5. Warning Mode Active Control Loop
  if (warningMode) {
    // Feature 4: Slow blinking active during last 5 seconds (every 250ms toggle)
    if (millis() - lastBlinkTime >= 250) {
      blinkState = !blinkState;
      digitalWrite(ledPin, blinkState);
      lastBlinkTime = millis();
    }

    // Feature 3: Live numeric countdown tracking
    if (millis() - lastCountTime >= 1000) {
      countdown--;
      if (countdown > 0) {
        Serial.println(countdown);
      }
      lastCountTime = millis();
    }

    // Feature 2 & 9: Countdown hits 0 -> Auto OFF activates
    if (countdown <= 0) {
      ledState = false;
      warningMode = false;
      digitalWrite(ledPin, LOW);
      
      autoOffCount++;
      totalOnTimeMs += (millis() - startTime); // Capture full 30-second runtime

      Serial.println();
      Serial.println("Sleep Mode Activated");
      Serial.println("Light OFF");
    }
  }
}

// Helper Function: Generates and prints analytics report format
void printReport(String sessionStatus) {
  // If the light is active right now, compute current elapsed time on the fly
  unsigned long dynamicSessionTime = ledState ? (millis() - startTime) : 0;
  unsigned long aggregateTimeSeconds = (totalOnTimeMs + dynamicSessionTime) / 1000;

  // Features 7 & 8: Math Formulas
  double energyUsedWh = LED_POWER_WATTS * ((double)aggregateTimeSeconds / 3600.0);
  double estimatedCostTk = energyUsedWh * COST_PER_WH;

  unsigned long averageSessionSeconds = 0;
  if (sessionCount > 0) {
    averageSessionSeconds = aggregateTimeSeconds / sessionCount;
  }

  Serial.println("========================================");
  Serial.println("SMART LIGHT ANALYTICS");
  Serial.println("========================================");
  Serial.println();
  Serial.println(sessionStatus);
  Serial.println();
  Serial.print("Touch Count          : "); Serial.println(touchCount);
  Serial.print("Total ON Time        : "); formatTimeDisplay(aggregateTimeSeconds, false); Serial.println();
  Serial.print("Energy Used          : "); Serial.print(energyUsedWh, 3); Serial.println(" Wh");
  Serial.print("Estimated Cost       : "); Serial.print(estimatedCostTk, 5); Serial.println(" Tk");
  Serial.print("Auto OFF Events      : "); Serial.println(autoOffCount);
  Serial.print("Average Session Time : "); formatTimeDisplay(averageSessionSeconds, true); Serial.println();
  Serial.println();
  Serial.println("========================================");
}

// Helper Function: Resets all monitoring variables to 0
void resetStatistics() {
  touchCount = 0;
  autoOffCount = 0;
  totalOnTimeMs = 0;
  
  if (ledState) {
    startTime = millis(); // Continuous baseline shift if light stays running
    sessionCount = 1;
  } else {
    sessionCount = 0;
  }
  twentyFourHourTimer = millis(); 
}

// Helper Function: Formats raw seconds cleanly matching the documentation style
void formatTimeDisplay(unsigned long totalSeconds, bool isAverageField) {
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;

  if (hours > 0) {
    Serial.print(hours); Serial.print("h ");
    Serial.print(minutes); Serial.print("m ");
    Serial.print(seconds); Serial.print("s");
  } 
  else if (minutes > 0) {
    Serial.print(minutes); Serial.print("m ");
    Serial.print(seconds); Serial.print("s");
  } 
  else {
    if (isAverageField) {
      Serial.print(seconds); Serial.print(" sec");
    } else {
      Serial.print(seconds); Serial.print("s");
    }
  }
}