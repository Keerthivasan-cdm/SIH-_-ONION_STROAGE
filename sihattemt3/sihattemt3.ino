#include "DHT.h"

// DHT11 configuration
#define DHTPIN 4      
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// NTC configuration
int ntc_pin  = 34;       // ADC1 pin (recommended: 34/35/36/39)
int ntc_R    = 11000;    // NTC nominal resistance at 25°C
int ntc_T    = 25;       // Nominal temperature
int beta     = 3950;     // From datasheet
int Series_R = 10000;    // Fixed resistor
int adc_max  = 4400;     // 12-bit ADC max value

// --- Fan pin ---
#define FAN_PIN 25  

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(FAN_PIN, OUTPUT);   // Fan control pin
  digitalWrite(FAN_PIN, LOW); // Fan OFF initially
  Serial.println("DHT11 + NTC Sensor Test");
}

void loop() {
  // --- DHT11 Readings ---
  float humidity = dht.readHumidity();
  float dhtTemp = dht.readTemperature(); // Celsius

  if (!isnan(humidity) && !isnan(dhtTemp)) {
    Serial.print("DHT11 -> Temperature: ");
    Serial.print(dhtTemp);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
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
  
  // --- FAN CONTROL ---
  if (tempC < 30) {  
    digitalWrite(FAN_PIN, HIGH);  // Fan ON
    Serial.println("FAN ON (Temp < 27°C)");
  } else {
    digitalWrite(FAN_PIN, LOW);   // Fan OFF
    Serial.println("FAN OFF (Temp >= 27°C)");
  }

  delay(2000);
}

// --- Steinhart Equation Function ---
double Steinhart(int Ro, int To, int b, float R) {
  float T = R / Ro;
  T = log(T);
  T /= b;
  T += 1.0 / (To + 273.15);
  T = 1.0 / T;
  T -= 273.15;
  return T-50;   // remove -43 offset unless you calibrate
}