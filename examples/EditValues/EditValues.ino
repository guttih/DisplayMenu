/**
 * @file EditValues.ino
 * @author guttih (gudjonholm@gmail.com)
 * @brief A program that creates a menu with three pages.
 * @version 0.1
 * @date 2021-08-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>

#include <DisplayMenu.h>

#include <FS.h>

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

/**
 * @brief Global variables in the program
 * 
 */
struct GLOBAL_STRUCT {
    double temperature = 0;   // Current temperature read from the Temperature sensor.
    double desiredTemp = 24;  // The desired temperature which should be fed as the setpoint to the PID controller.
    double coldValveFlow = 12.34; // A number from 0 to 100 describing the opening of the COLD valve, 0 is fully closed and 100 fully open.
    double hotValveFlow = 56.78;  // A number from 0 to 100 describing the opening of the HOT  valve, 0 is fully closed and 100 fully open.
} values;


#define CALIBRATION_FILE "/TouchCalData3"


const bool invertColors = false;

#define REPEAT_CAL true

TFT_eSPI tft = TFT_eSPI();


DisplayMenu menu = DisplayMenu(&tft);

void touch_calibrate();
void setupMenu();

#include "menusetup.h"

unsigned long updateTempTimer = 0;

void setup()
{
  Serial.begin(115200);

  if (tft.width() >= 480)
    tft.setRotation(0);  
  touch_calibrate();
  
  menu.invertColors(invertColors);
  setupMenu();
  menu.showPage(1);
  updateTempTimer = millis() + 10;
}

void loop()
{
  menu.update();

  if (millis() > updateTempTimer) {
    globalTemperature+=0.1;
    updateTempTimer = millis() + 10;
      pTempShowButton->draw(); //drawn only when pageValve is visable
  }
}

void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin())
  {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE))
  {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f)
      {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL)
  {
    // calibration data valid
    tft.setTouch(calData);
  }
  else
  {
    tft.invertDisplay(false);
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL)
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
