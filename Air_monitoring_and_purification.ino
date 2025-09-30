#include <DHT.h>
#include <DHT_U.h>

#include <LiquidCrystal.h>


// LCD connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// DHT11 setup
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Sensor pins
#define AIR_QUALITY_PIN A0    // HIH-5030 as air quality sensor
#define LIGHT_SENSOR_PIN 10   // Ambient light sensor as dust sensor
#define FAN_RELAY_PIN 7       // Relay control for fan motor

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Set relay pin as output
  pinMode(FAN_RELAY_PIN, OUTPUT);
  
  // Turn on fan initially
  digitalWrite(FAN_RELAY_PIN, HIGH);
  
  // Welcome message
  lcd.print("Air Quality Sys");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read all sensors
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int airQualityRaw = analogRead(AIR_QUALITY_PIN);
  int lightValue = digitalRead(LIGHT_SENSOR_PIN); // Assuming digital light sensor
  
  // Convert HIH-5030 reading to 0-100 air quality scale
  // HIH-5030 outputs 0.8V to 3.9V for 0% to 100% RH
  // For simulation, we'll map the analog reading to 0-100
  int airQuality = map(airQualityRaw, 0, 1023, 0, 100);
  
  // Display temperature and humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  // Display air quality and light (dust) status
  lcd.setCursor(0, 1);
  lcd.print("AQ:");
  lcd.print(airQuality);
  lcd.print("% L:");
  lcd.print(lightValue ? "Hi " : "Lo ");
  
  // Control fan based on air quality (example threshold)
  if (airQuality > 70) {  // If air quality is poor
    digitalWrite(FAN_RELAY_PIN, HIGH);  // Turn on fan
  } else {
    digitalWrite(FAN_RELAY_PIN, LOW);   // Turn off fan
  }
  
  // Add short delays between readings
  delay(2000);  // 2-second delay between updates
  
  // Optional: Clear the second line occasionally to prevent ghosting
  static unsigned long lastClear = 0;
  if (millis() - lastClear > 10000) {  // Every 10 seconds
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear line
    lastClear = millis();
  }
}
