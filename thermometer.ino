#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

const int tftCs = 7;
const int tftDc = 39;
const int tftRst = 40;
const int tftBacklight = 45;
const int tftI2cPower = 21;
const int sensorPin = 8;
const int ledPin = 13;

const unsigned int temperatureStringSize = 20;

const int samplePeriod = 5000;
const int lowTempLedPeriod = 1000;
const int highTempLedPeriod = 200;
const int loopPeriod = 200;

const int sampleCount = samplePeriod / loopPeriod;
const int lowTempLedCount = lowTempLedPeriod / loopPeriod;
const int highTempLedCount = highTempLedPeriod / loopPeriod;

const float lowTemp = 19.0f;
const float highTemp = 21.0f;

const char degreeSymbol = 248;

int counter = 0;
float temperature = 0.0f;
char temperatureString[temperatureStringSize];
int ledState = LOW;

Adafruit_ST7789 tft = Adafruit_ST7789(tftCs, tftDc, tftRst);

void setup(void) {
  // turn on backlite
  pinMode(tftBacklight, OUTPUT);
  digitalWrite(tftBacklight, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(tftI2cPower, OUTPUT);
  digitalWrite(tftI2cPower, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(3);

  // initialize LED
  pinMode(ledPin, OUTPUT);
  
  // get first temperature reading
  updateTemperature();
}

void loop() {
  counter++;
  delay(loopPeriod);

  if (temperature < lowTemp && counter % lowTempLedCount == 0) {
    toggleLed();
  }

  if (temperature > highTemp && counter % highTempLedCount == 0) {
    toggleLed();
  }

  if (counter == sampleCount) {
    updateTemperature();
    counter = 0;
  }
}

void updateTemperature() {
    temperature = getTemperature();
    sprintf(temperatureString, "Temperature: %.1f%cC", temperature, degreeSymbol);
    drawText(temperatureString, ST77XX_WHITE);
    if (temperature >= lowTemp && temperature <= highTemp) {
      ledOff();
    }
}

float getTemperature() {
  int sensorValue = analogRead(sensorPin);
  float voltage = sensorValue / 8192.0 * 2.7;
  return (voltage - 0.424) / 0.00625;
}

void toggleLed() {
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
}

void ledOff() {
  ledState = LOW;
  digitalWrite(ledPin, ledState);
}

void drawText(char *text, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 60);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(2);
  tft.print(text);
}
