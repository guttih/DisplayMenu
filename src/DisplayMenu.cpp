#include "DisplayMenu.h"

DisplayMenu::DisplayMenu(TFT_eSPI *tft, uint16_t fillColor)
{
    init(tft, fillColor);
}

void DisplayMenu::init(TFT_eSPI *tft, uint16_t fillColor)
{
    _tft = tft;
    _fillColor = fillColor;

    _tft->init();
    // // Set the rotation before we calibrate
    _tft->setRotation(1);
    // // Clear screen
    _tft->fillScreen(fillColor);
    _tft->setFreeFont(&FreeMonoBold9pt7b); 

    //_tft->setFreeFont(&FreeMono9pt7b);
    //_tft->setFreeFont(&FreeSans9pt7b);
    //_tft->setFreeFont(&FreeSansBold9pt7b);
    //_tft->setFreeFont(&FreeSansBoldOblique9pt7b);
    // _tft->setFreeFont(&FreeSerif9pt7b); //fallegur en ekki vel lesanlegur
    //_tft->setFreeFont(&FreeSerifBold9pt7b);
    //_tft->setFreeFont(&FreeSerifBoldItalic9pt7b);
    //_tft->setFreeFont(&FreeSerifItalic9pt7b);
    
    
    _touch.pressed = false;
    _touch.x = 0;
    _touch.y = 0;
    _visablePage = -1;
}

void DisplayMenu::showPage(int index)
{

    _visablePage = index;
    DisplayPage *pPage = getPage(_visablePage);
    pPage->show();
}

void DisplayMenu::showPage(DisplayPage *pPage)
{

    int index = pages.indexOf(pPage);
    showPage(index);
}

DisplayPage *DisplayMenu::getVisablePage()
{

    if (_visablePage < 0)
        return NULL;

    return getPage(_visablePage);
}

DisplayPage *DisplayMenu::getPage(int index)
{
    return pages.get(index);
}


DisplayPage * DisplayMenu::addPage()
{
    return addPage(_fillColor);
}

DisplayPage * DisplayMenu::addPage(uint16_t fillColor)
{
     DisplayPage page(_tft, this, fillColor);
    return pages.add(page)? getLastPage() : NULL;
}

DisplayPage * DisplayMenu::addPage(DisplayPage page)
{
    return pages.add(page)? getLastPage() : NULL;
}

DisplayPage *DisplayMenu::getLastPage()
{
    int size = pages.size();
    if (size < 1)
        return NULL;

    return pages.get(size - 1);
}

bool DisplayMenu::update()
{
    bool didUpdate = false;

    _touch.pressed = _tft->getTouch(&_touch.x, &_touch.y);

    if (_touch.pressed)
    {
        DisplayPage *pCurrentPage = getVisablePage();
        if (pCurrentPage)
        {
            DisplayButton *btn = pCurrentPage->getPressedButton(_touch.x, _touch.y);
            pCurrentPage->drawTouchButtonsState();
            delay(100);
            didUpdate = true;
            if (btn)
            {
                if (btn->_values.allowOnlyOneButtonPressedAtATime)
                {
                    do
                    {
                        delay(50);
                        _touch.pressed = _tft->getTouch(&_touch.x, &_touch.y);
                        if (!_touch.pressed)
                        {
                            delay(10);
                            _touch.pressed = _tft->getTouch(&_touch.x, &_touch.y);
                        }
                    } while (_touch.pressed); //wait until no touch, then execute

                    //make button not inverted
                    btn->resetPressState();
                    btn->draw();
                }
                btn->executeCommand();
            }
        }
    }
    return didUpdate;
}