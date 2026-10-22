#define BLYNK_TEMPLATE_ID "TMPL3-0ZQPvie"
#define BLYNK_TEMPLATE_NAME "check"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <math.h>

// --- WiFi Credentials ---
char ssid[] = "YOUR_WIFI_SSID";      // <-- change this
char pass[] = "YOUR_WIFI_PASSWORD";  // <-- change this

// --- DHT11 configuration ---
#define DHTPIN 4      
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// --- NTC configuration ---
int ntc_pin  = 34;       // ADC1 pin (recommended: 34/35/36/39)
int ntc_R    = 11000;    // NTC nominal resistance at 25°C
int ntc_T    = 25;       // Nominal temperature
int beta     = 3950;     // From datasheet
int Series_R = 10000;    // Fixed resistor
int adc_max  = 4400;     // ESP32 12-bit ADC max value

// --- Fan pin ---
#define FAN_PIN 25  

BlynkTimer timer;

// --- Steinhart Equation Function ---
double Steinhart(int Ro, int To, int b, float R) {
  float T = R / Ro;
  T = log(T);
  T /= b;
  T += 1.0 / (To + 273.15);
  T = 1.0 / T;
  T -= 273.15;
  return T-50;   // no offset
}

// --- Function to Read Sensors + Control Fan ---
void sendSensor() {
  // --- DHT11 Readings ---
  float humidity = dht.readHumidity();
  float dhtTemp = dht.readTemperature(); // Celsius

  if (!isnan(humidity) && !isnan(dhtTemp)) {
    Serial.print("DHT11 -> Temperature: ");
    Serial.print(dhtTemp);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Blynk.virtualWrite(V0, dhtTemp);   // DHT11 Temp
    Blynk.virtualWrite(V1, humidity);  // DHT11 Humidity
  } else {
    Serial.println("DHT11 failed to read!");
  }

  // --- NTC Readings ---
  int adcVal = analogRead(ntc_pin);
  float voltage = adcVal * 3.3 / adc_max;
  float R_ntc = Series_R * (3.3 / voltage - 1.0);
  float tempC = Steinhart(ntc_R, ntc_T, beta, R_ntc);

  Serial.print("NTC  -> ADC Value: "); Serial.print(adcVal);
  Serial.print(" | Voltage: "); Serial.print(voltage, 3); Serial.print(" V");
  Serial.print(" | Resistance: "); Serial.print(R_ntc, 1); Serial.print(" Ohm");
  Serial.print(" | Temperature: "); Serial.print(tempC, 2); Serial.println(" °C");

  Blynk.virtualWrite(V2, tempC);  // Send NTC Temp to Blynk

  // --- FAN CONTROL ---
  if (tempC < 30) {  
    digitalWrite(FAN_PIN, HIGH);  // Fan ON
    Serial.println("FAN ON (Temp < 30°C)");
    Blynk.virtualWrite(V3, 1);    // Optional: send Fan status
  } else {
    digitalWrite(FAN_PIN, LOW);   // Fan OFF
    Serial.println("FAN OFF (Temp >= 30°C)");
    Blynk.virtualWrite(V3, 0);    // Optional: send Fan status
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(FAN_PIN, OUTPUT);   // Fan control pin
  digitalWrite(FAN_PIN, LOW); // Fan OFF initially

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("DHT11 + NTC + FAN with Blynk (Template: check)");

  timer.setInterval(1000L, sendSensor);  // Run every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
