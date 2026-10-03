/**
 * @file wifi-blink.ino
 * @brief ESP32 Wi-Fi station connection with FreeRTOS background LED blinker task.
 */

#include <WiFi.h>

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const int   LED_PIN  = 2; // Onboard LED on most ESP32 DevKit boards

// Dedicated FreeRTOS background task for LED blinking
void TaskBlink(void *pvParameters) {
    (void)pvParameters;
    pinMode(LED_PIN, OUTPUT);
    while (1) {
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n[ESP32] Booting...");

    // Spawn non-blocking background task on Core 1
    xTaskCreatePinnedToCore(
        TaskBlink,   // Task function
        "BlinkTask", // Name
        2048,        // Stack depth in words
        NULL,        // Parameters
        1,           // Priority
        NULL,        // Handle
        1            // CPU Core
    );

    // Connect to Wi-Fi network in foreground
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\n[ESP32] Connected successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Main loop remains responsive for telemetry or web requests
    vTaskDelay(pdMS_TO_TICKS(1000));
}
