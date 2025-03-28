#include <LiquidCrystal.h> // Include the library to control the LCD

// Initialize the LCD with its pin connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Define pin connections for various sensors and output devices
const int pulseSensorPin = 8;  // Pin for heartbeat sensor
const int tempSensorPin = A0;  // Pin for temperature sensor (LM35)
const int greenLed = 11;       // Green LED (indicating a stable patient)
const int yellowLed = 12;      // Yellow LED (indicating a medium-risk patient)
const int redLed = 10;         // Red LED (indicating a high-risk patient)
const int buzzer = 13;         // Buzzer for alert system
const int startButton = 9;     // Start button to begin monitoring
const int stopButton = 0;      // Stop button to stop monitoring
const int stopBuzzerButton = A5; // Stop buzzer button

// Variables to manage monitoring status
bool monitoring = false;       // Flag to check if monitoring is active
unsigned long startTime;       // Variable to store monitoring start time
int heartBeats = 0;            // Counter for heartbeats detected
unsigned long lastBeatTime = 0;// Stores the time of the last detected heartbeat
bool pulseDetected = false;    // Variable to track heartbeat detection state

// Timing variables
unsigned long previousMillis = 0; // Stores the last update time
const unsigned long interval = 2000; // Update every 2 seconds

// Buzzer mute flag
bool buzzerMuted = false;

void setup() {
    Serial.begin(9600);       // Initialize serial communication for debugging
    lcd.begin(20, 4);         // Initialize a 20x4 character LCD screen
    pinMode(pulseSensorPin, INPUT);  
    pinMode(tempSensorPin, INPUT);   
    pinMode(greenLed, OUTPUT);       
    pinMode(yellowLed, OUTPUT);      
    pinMode(redLed, OUTPUT);         
    pinMode(buzzer, OUTPUT);         
    pinMode(startButton, INPUT_PULLUP);
    pinMode(stopButton, INPUT_PULLUP);
    pinMode(stopBuzzerButton, INPUT_PULLUP);

    // Display startup message on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Health Monitor");
    delay(1000);  
    lcd.clear();
}

void loop() {
    unsigned long currentMillis = millis();

    // Start button
    if (digitalRead(startButton) == LOW) {
        delay(100);
        monitoring = true;
        startTime = millis();
        heartBeats = 0;
        lcd.clear();
        noTone(buzzer);
    }

    // Stop button
    if (digitalRead(stopButton) == LOW) {
        delay(100);
        monitoring = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Monitoring Stopped");
        digitalWrite(greenLed, LOW);
        digitalWrite(yellowLed, LOW);
        digitalWrite(redLed, LOW);
        noTone(buzzer);
        delay(1000);
        lcd.clear();
    }

    // Stop buzzer button
    if (digitalRead(stopBuzzerButton) == LOW) {
        delay(100);
        buzzerMuted = true;
        noTone(buzzer);
    }

    if (monitoring) {
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis; // Reset timer

            // Read sensors
            int rawTemp = analogRead(tempSensorPin);
            float voltage = rawTemp * (5.0 / 1023.0);
            float temperature = voltage * 100.0;

            int pulseValue = digitalRead(pulseSensorPin);
            if (pulseValue == HIGH && !pulseDetected) {
                pulseDetected = true;
                heartBeats++;
                lastBeatTime = millis();
            } 
            if (pulseValue == LOW) { 
                pulseDetected = false;
            }

            int heartRate = (heartBeats * 6);
            int riskScore = (heartRate / 2) + (temperature * 2);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("HB per Min: ");
            lcd.print(heartRate);
            lcd.setCursor(0, 1);
            lcd.print("Body Temp: ");
            lcd.print(temperature);
            lcd.print(" C");
            lcd.setCursor(0, 2);
            lcd.print("Risk Score: ");
            lcd.print(riskScore);

            // LED and buzzer alerts
            if (riskScore <= 80) {
                digitalWrite(greenLed, HIGH);
                digitalWrite(yellowLed, LOW);
                digitalWrite(redLed, LOW);
                noTone(buzzer);
                buzzerMuted = false;  // Reset mute flag when safe
            } 
            else if (riskScore > 80 && riskScore <= 100) {
                digitalWrite(greenLed, LOW);
                digitalWrite(yellowLed, HIGH);
                digitalWrite(redLed, LOW);
                if (!buzzerMuted) {
                    tone(buzzer, 2000);
                }
            } 
            else {
                digitalWrite(greenLed, LOW);
                digitalWrite(yellowLed, LOW);
                digitalWrite(redLed, HIGH);
                if (!buzzerMuted) {
                    tone(buzzer, 4000);
                }
            }
        }
    }
}
