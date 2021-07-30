/**
 * @file menusetup.h
 * @author guttih (gudjonholm@gmail.com)
 * @brief Global program functions for setting upp the menu and pages for the TFT display.
 * @date 2021-07-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef MENUSETUPH
#define MENUSETUPH

#include <Arduino.h>
#include "DisplayMenu.h"
#include "DisplayPage.h"
#include "DisplayButton.h"
#include "editpagesetup.h"

const uint16_t TFT_BUTTON_OUTLINE = tft.color565(115, 149, 125);
const uint16_t TFT_BUTTON_FILL = tft.color565(48, 73, 47);
const uint16_t TFT_BUTTON_TEXT = TFT_GOLD;

void addPageMenu()
{
    menu.addPage();
}

void showPageEditTemperature(DisplayButton *menuButton)
{

    //about to open edit temperature page
    DisplayMenu *pMenu = menuButton->getPage()->getMenu();
    DisplayPage *pEditPage = pMenu->getPage(2);

    DisplayButton *valueButton = pEditPage->getLastButton();
    valueButton->setPageToOpen(pMenu->getPage(0));
    valueButton->setLinkToValue(&globalTemperature, "Temperature");
    allowDouble = true;
    allowMinus = true;
    pMenu->showPage(2);
}

void showPageEditGlobalDouble(DisplayButton *menuButton)
{

    DisplayMenu *pMenu = menuButton->getPage()->getMenu();
    DisplayPage *pEditPage = pMenu->getPage(2);

    DisplayButton *valueButton = pEditPage->getLastButton();
    valueButton->setPageToOpen(pMenu->getPage(0));
    valueButton->setLinkToValue(&globalValueDouble, "Global double");

    allowDouble = true;
    allowMinus = true;
    pMenu->showPage(2);
}

void showPageEditGlobalLong(DisplayButton *menuButton)
{

    DisplayMenu *pMenu = menuButton->getPage()->getMenu();
    DisplayPage *pEditPage = pMenu->getPage(2);

    DisplayButton *valueButton = pEditPage->getLastButton();

    valueButton->setPageToOpen(pMenu->getPage(0));
    valueButton->setLinkToValue(&globalValueLong, "Global long");

    allowDouble = false;
    allowMinus = true;
    pMenu->showPage(2);
}

void onDrawPageValves(DisplayPage *pPage)
{

    if (values.coldValveFlow < 0)
        values.coldValveFlow = 0;
    if (values.coldValveFlow > 100)
        values.coldValveFlow = 100;
    if (values.hotValveFlow < 0)
        values.hotValveFlow = 0;
    if (values.hotValveFlow > 100)
        values.hotValveFlow = 100;
    tft.setFreeFont(&FreeSans12pt7b);

    int16_t screenXCenter = tft.width() / 2;

    uint8_t oldDatum = tft.getTextDatum();

    tft.setTextDatum(TL_DATUM);

    tft.setTextColor(TFT_CYAN);
    tft.drawString("Cold", 36, 40);

    int32_t h = 20, w = 100, x = 20, y = 70;
    tft.drawRect(x, y, w, h, pPage->getFillColor());
    tft.fillRect(x, y, w, h, pPage->getFillColor());
    tft.drawString((String(values.coldValveFlow, 2)) + "%", x, y);

    tft.setTextColor(tft.color24to16(0xfb745b));
    tft.drawString("Hot", 36 + 208, 40);

    x = 220, y = 70;
    tft.drawRect(x, y, w, h, pPage->getFillColor());
    tft.fillRect(x, y, w, h, pPage->getFillColor());
    tft.drawString((String(values.hotValveFlow, 2)) + "%", x, y);

    tft.setTextColor(TFT_GOLD);
    tft.setTextDatum(C_BASELINE);

    tft.setFreeFont(&FreeSans9pt7b);
    tft.drawString("Temperature", screenXCenter, 200);
    tft.setTextDatum(oldDatum);
}

void addValve(DisplayPage *pPage, bool hotValve)
{
    const int buttonWidth = 42;
    const int buttonHeight = 39;
    const int buttonMarginX = 10;
    const int buttonMarginY = 100;
    const int buttonPaddingX = 3;
    const int buttonPaddingY = 3;
    const int x = hotValve ? 208 : 5;
    double *pPlowValue = hotValve ? &values.hotValveFlow : &values.coldValveFlow;

    int btnCount = -1;
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "<", pPlowValue, -0.1);
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, ">>", pPlowValue, 1);
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "<<<", pPlowValue, -10);
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, ">", pPlowValue, 0.1);
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "<<", pPlowValue, -1);
    ++btnCount;
    pPage->addIncrementButton(buttonMarginX + ((btnCount % 2) * (buttonPaddingX + buttonWidth)) + x, buttonMarginY + (btnCount % 3) * (buttonHeight + buttonPaddingY), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, ">>>", pPlowValue, 10);
}

void onDrawTemperatureButton(DisplayButton *ptrButton)
{

    ptrButton->setText(toString(*ptrButton->getLinkedValue()));
}
void addPageValves()
{
    DisplayPage *pPage = menu.addPage();
    pPage->registerOnDrawEvent(onDrawPageValves);

    addValve(pPage, false);
    addValve(pPage, true);

    //temperatue button used to show show the temperature and if pressed edit the temperature
    const int w = 100;
    pTempShowButton = pPage->addFunctionButton(((tft.width() - w) / 2), 210, 100, 20, pPage->getFillColor(), pPage->getFillColor(), TFT_GOLD, 1, NULL, showPageEditTemperature);
    pTempShowButton->setLinkToValue(&globalTemperature, "value missing!");
    pTempShowButton->setDatum(MC_DATUM, 0, 0);
    pTempShowButton->registerOnDrawEvent(onDrawTemperatureButton);
}

void setupMenu()
{
    Serial.println("Global addresses");
    Serial.printf(" - globalValueDouble: %p\n", (void *)&globalValueDouble);
    Serial.printf(" - globalValueLong: %p\n", (void *)&globalValueLong);
    Serial.printf(" - temp: %p\n", (void *)&globalTemperature);

    addPageMenu();   //index 0
    addPageValves(); //index 1

    addPageEditValue(&menu); //index 2

    menu.getPage(1)->addPageButton((tft.width() - 100) / 2, 0, 100, 50, tft.color565(115, 149, 125), tft.color565(48, 73, 47), TFT_GOLD, 1, "Menu", menu.getPage(0));

    //Add menu buttons
    const int buttonWidth = 170;
    const int buttonHeight = 50;
    const int buttonMargin = 20;
    const int x = (tft.width() - buttonWidth) / 2;

    DisplayPage *pPage = menu.getPage(0);
    pPage->addPageButton(x, buttonMargin, buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "Valves", menu.getPage(1));
    pPage->addFunctionButton(x, buttonMargin + 1 * (buttonMargin + buttonHeight), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "Edit double", showPageEditGlobalDouble);
    pPage->addFunctionButton(x, buttonMargin + 2 * (buttonMargin + buttonHeight), buttonWidth, buttonHeight, TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, "Edit long", showPageEditGlobalLong);

    menu.showPage(1);
    allowMinus = true;
    allowDouble = false;
}

#endif