#ifndef DISPLAYMENU_H
#define DISPLAYMENU_H

#include "arduino.h"
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "DisplayPage.h"
#include "DisplayPageList.h"

struct TOUCHED_STRUCT {
    uint16_t x;
    uint16_t y;
    bool pressed;

};

class DisplayMenu
{
private:
    TFT_eSPI *_tft;
    int _visablePage;
    uint16_t _fillColor;  //default fill color for pages
    TOUCHED_STRUCT _touch;
    DisplayPageList pages;
    unsigned long myTouchTimer;
    unsigned long myTouchDelay;

    void init(TFT_eSPI *tft, uint16_t fillColor);
    
public:
    DisplayMenu(TFT_eSPI *tft, uint16_t fillColor = TFT_BLACK);
    DisplayPage * addPage();
    DisplayPage * addPage(uint16_t fillColor);
    DisplayPage * addPage(DisplayPage page);
    DisplayPage *getPage(int index);

    //called when a page is beeing made visable;
    void showPage(int index);
    void showPage(DisplayPage *pPage);
    DisplayPage *getVisablePage();
    
    /**
     * @brief Get the Visable Page Index 
     * 
     * @return int if no page is visable -1 is returned
     */
    int getVisablePageIndex() { return _visablePage; };
    DisplayPage*  getLastPage();

    //DisplayPage*   getVisablePageIndex() { return _visablePage; };
    /**
     * @brief checks if a button was pressed and updates it's value and runs it's associated actions. 
     * 
     * @return true a button was pressed and a button status did change.
     * @return false no button was pressed.
     */
    bool update();
};


#endif