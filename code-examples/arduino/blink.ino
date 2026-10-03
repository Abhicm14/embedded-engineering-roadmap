/**
 * @file blink.ino
 * @brief Standard Arduino framework baseline LED blinker.
 */

const int LED_PIN = 13; // Built-in LED on most Arduino boards

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Arduino Blink Initialized");
}

void loop() {
    digitalWrite(LED_PIN, HIGH); // Turn LED on
    delay(500);                  // Wait 500 ms
    digitalWrite(LED_PIN, LOW);  // Turn LED off
    delay(500);                  // Wait 500 ms
}
