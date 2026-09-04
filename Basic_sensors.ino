#include <Wire.h> 
#include <LiquidCrystal_I2C_Hangul.h> // Updated to use your installed library
#include <DHT.h>

// Set the LCD address to 0x27 based on your scanner result
LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2); 

// --- Sensor Pin Mapping ---
#define DHTPIN        2     // DHT11 Data Pin -> Arduino D2
#define DHTTYPE       DHT11 
#define VIBRATION_PIN 3     // Vibration Sensor Digital Out -> Arduino D3
#define SOUND_PIN     A0    // Sound Sensor Analog Out -> Arduino A0

DHT dht(DHTPIN, DHTTYPE);

// Screen alternation variables
unsigned long previousMillis = 0;
const long displayInterval = 2500; // Switch display pages every 2.5 seconds
bool displayPage = 0;             // 0 = Climate Page, 1 = Sound/Vibration Page

void setup() {
  // Initialize the I2C LCD Screen
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Turns on the screen backlight

  // Initialize Sensors
  dht.begin();
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);

  // Initial greeting text
  lcd.setCursor(0, 0);
  lcd.print("UNO R4 ONLINE");
  lcd.setCursor(0, 1);
  lcd.print("READING DATA...");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // Read sensor inputs continuously
  int vibStatus = digitalRead(VIBRATION_PIN);
  int soundRaw = analogRead(SOUND_PIN);
  float temp = dht.readTemperature(); 
  float hum = dht.readHumidity();

  // Fallback if the DHT sensor is slow or disconnected
  if (isnan(temp) || isnan(hum)) {
    temp = 0.0;
    hum = 0.0;
  }

  // Handle page flipping timer
  if (currentMillis - previousMillis >= displayInterval) {
    previousMillis = currentMillis;
    displayPage = !displayPage; 
    lcd.clear(); // Clear old text to prevent trailing text bugs
  }

  // Render current page layout
  if (displayPage == 0) {
    // PAGE 1: Climate Matrix
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(hum, 0);
    lcd.print("%");
  } 
  else {
    // PAGE 2: Dynamics Indicators
    lcd.setCursor(0, 0);
    lcd.print("Vib: ");
    if (vibStatus == HIGH) {
      lcd.print("ALERT!      ");
    } else {
      lcd.print("STABLE      ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Sound Vol: ");
    lcd.print(soundRaw);
    lcd.print("    "); 
  }
}