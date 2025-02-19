#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Telegram Bot credentials
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_TELEGRAM_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// DHT Sensor
#define DHTPIN 4 // GPIO4 for DHT sensor
#define DHTTYPE DHT11  // or DHT22
DHT dht(DHTPIN, DHTTYPE);

// Sensors
#define SOUND_SENSOR_PIN 34  // GPIO34 for sound sensor
#define PIR_SENSOR_PIN 27    // GPIO27 for motion sensor
#define LED_PIN 2            // GPIO2 for alert LED

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Connected!");
    
    dht.begin();
    pinMode(SOUND_SENSOR_PIN, INPUT);
    pinMode(PIR_SENSOR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    client.setInsecure(); // Required for Telegram SSL
    
    bot.sendMessage(CHAT_ID, "Baby Monitor System Activated!", "");
}

void loop() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int sound = analogRead(SOUND_SENSOR_PIN);
    int motion = digitalRead(PIR_SENSOR_PIN);

    Serial.print("Temp: "); Serial.print(temp);
    Serial.print("C, Humidity: "); Serial.print(hum);
    Serial.print("%, Sound: "); Serial.print(sound);
    Serial.print(", Motion: "); Serial.println(motion);

    if (motion == HIGH) {
        digitalWrite(LED_PIN, HIGH);
        bot.sendMessage(CHAT_ID, "Motion detected! Baby is moving.", "");
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    if (sound > 800) { // Adjust threshold as needed
        bot.sendMessage(CHAT_ID, "Loud noise detected! Baby might be crying.", "");
    }

    if (temp > 30) { // Adjust threshold
        bot.sendMessage(CHAT_ID, "Warning: High temperature detected!", "");
    }

    delay(5000); // Update every 5 seconds
}
