#include <M5Core2.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000

TFT_eSPI tft=TFT_eSPI();
PulseOximeter pox;
uint32_t tsLastReport = 0;
float heartrate,oxygenrate;

void onBeatDetected()
{
//This message will be printed on the serial monitor when the beat is detected
Serial.println("Beat!");
}

void readvalues()
{
  heartrate = pox.getHeartRate();  //Reading heartrate values from the sensor 
  oxygenrate = pox.getSpO2();// Reading the the oxygen saturation values from the sensor
  
}

void displayvalues()
{
 //The information is printed on the serial monitor
  Serial.print("Heart rate:");
  Serial.print(pox.getHeartRate());
  Serial.print("bpm / SpO2:");
  Serial.print(pox.getSpO2());
  Serial.println("%");

  //printing on the m5
    M5.Lcd.clear();
    M5.Lcd.setCursor(0,0);
    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0,0,340,240,TFT_RED); 
    M5.Lcd.fillRect(0, 0, 340, 240, RED); 
    M5.Lcd.print("\nHeartRate:");
    M5.Lcd.print(heartrate);
    M5.Lcd.println("bpm");
    M5.Lcd.setCursor(0,100);
    
   M5.Lcd.fillRect(0, 100, 340, 240, PINK); 
    M5.Lcd.print("\n\n    SpO2:");
    M5.Lcd.print(oxygenrate);
    M5.Lcd.println("%"); 
 }

void setup()
{
    M5.begin(); // Turning on the M5 Stack device
    // Setting up how the screen and font size when the infromarion is displayed
    M5.Lcd.setTextSize(3);
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.fillScreen(RED);
    M5.Lcd.setCursor(0, 50);
    //Initial setup of the serial monitor and plotter
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
    // Initialize sensor
    if (!pox.begin()) {
    // If sensor was not found, print the message below
    Serial.println("SENSOR NOT DETECTED!!!!!");
    M5.Lcd.println("Press Button A to learn about our procuct");
    M5.Lcd.println("Press Button B to read values");    
    while(true);
    }
    else {
    // If sensor was not found, print the message
    Serial.println("SENSOR WAS DETECTED!!");
    }
    //LED Configuration
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void vibrate()
{
  if (heartrate >70||oxygenrate>110) 
   {
      M5.Axp.SetLDOEnable(3, true);  // Open the vibration.   
      delay(100);
      M5.Axp.SetLDOEnable(3, false);
    delay(100);
   M5.Spk.DingDong();  // Play the DingDong sound.  
   }    
  }  
void loop()
{
//We will update both the sensor and M5Stack for evrytime we need to take a new pulse isdeteected
  pox.update();
  M5.update();
  //reading the heartrate and oxygen rate values
  readvalues();
  
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
  //displaying the values 
  displayvalues();
  //vibrate 
  vibrate();
  tsLastReport = millis();
  }  
}
