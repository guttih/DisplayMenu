
#include <Arduino.h>

#include <DisplayMenu.h>

#include <FS.h>

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

#define CALIBRATION_FILE "/TouchCalData3"

#define REPEAT_CAL true

TFT_eSPI tft = TFT_eSPI();


void touch_calibrate();


DisplayMenu menu = DisplayMenu(&tft);
void setup()
{
  const uint16_t buttonWidth = 120;
  const uint16_t buttonHeight = 50;
  const uint16_t centerHorizontal = (tft.width() - buttonWidth)/2;
  const uint16_t centerVertical   = (tft.height() - buttonHeight)/2;

  touch_calibrate();
  DisplayPage *pMainMenu, *pSecondMenu, *pThirdMenu;
  DisplayLabel *pLabel;
  pMainMenu   = menu.addPage();   //adding page at index 0
  pSecondMenu = menu.addPage();   //adding page at index 1
  pThirdMenu  = menu.addPage();   //adding page at index 2

  // Adding labels to pages
  pLabel = pMainMenu->addPageLabel(centerHorizontal, 0, buttonWidth, buttonHeight, pMainMenu->getFillColor(), pMainMenu->getFillColor(), TFT_YELLOW, 1, "Main menu");
  pLabel->setTextAlign(ALIGN_CENTER, 0, 0);

  pLabel = pMainMenu->addPageLabel(centerHorizontal, tft.height()-buttonHeight, buttonWidth, buttonHeight, pMainMenu->getFillColor(), pMainMenu->getFillColor(), TFT_ORANGE, 1, "Hello world demo");
  pLabel->setTextAlign(ALIGN_CENTER, 0, 0);
  pSecondMenu->addPageLabel(centerHorizontal, 0, buttonWidth, buttonHeight, pMainMenu->getFillColor(), pMainMenu->getFillColor(), TFT_YELLOW, 1, "Page 1");
  //You can also get a page by page index like this
  menu.getPage(2)->addPageLabel(centerHorizontal, 0, buttonWidth, buttonHeight, pMainMenu->getFillColor(), pMainMenu->getFillColor(), TFT_YELLOW, 1, "Page 2");


  //add a open page buttons to main menu;
  pMainMenu->addPageButton(centerHorizontal, centerVertical - buttonHeight/1.5, buttonWidth, buttonHeight, TFT_WHITE, TFT_RED, TFT_GOLD, 1, "Page 1",  menu.getPage(1));
  pMainMenu->addPageButton(centerHorizontal, centerVertical + buttonHeight/1.5, buttonWidth, buttonHeight, TFT_WHITE, TFT_RED, TFT_GOLD, 1, "Page 2",  menu.getPage(2));

  //add a open main menu button to second page
  pSecondMenu->addPageButton(centerHorizontal, centerVertical, buttonWidth, buttonHeight, TFT_WHITE, TFT_RED, TFT_GOLD, 1, "Main menu",  menu.getPage(0));
  
  //add a open main menu button to third page
  pThirdMenu->addPageButton(centerHorizontal, centerVertical, buttonWidth, buttonHeight, TFT_WHITE, TFT_RED, TFT_GOLD, 1, "Main menu",  menu.getPage(0));
  menu.showPage(0);
}

void loop()
{
  menu.update();

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
