/**
 * @file editpagesetup.h
 * @author guttih (gudjonholm@gmail.com)
 * @brief Creates a page that can modify a number
 * @date 2021-07-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef EDITPAGESETUPH
#define EDITPAGESETUPH

#include "DisplayMenu.h"

double globalValueDouble = 123.5;
double globalValueLong = 123;
double globalTemperature = 22.97;
DisplayButton *pTempShowButton = NULL;

bool allowDouble,
    allowMinus;

//want full precision but no ending zeros and no ending dot
String removeUnNecessaryDoubleEnding(String str)
{

    if (str.indexOf('.') > -1)
    {
        int len = str.length();
        while (len > 1 && (str.charAt(len - 1) == '0'))
        {
            str.remove(len - 1, 1);
            len = str.length();
        }
    }

    if (str.endsWith("."))
        str.remove(str.length() - 1, 1);

    return str;
}

String toString(double num)
{
    char output[50];

    snprintf(output, 50, "%.9f", num);
    return removeUnNecessaryDoubleEnding(output);
}

/**
 * @brief checks if a string can be converted to and from a double number, resulting in the exactly the same string
 * 
 * @param str a string to be tested and remove un necessary endings like 0 and .
 * @return String if success a string which can be converted to double and if fail an empty string:
 */
String convertToDoubleAndBackToSameString(String str)
{
    double d = str.toDouble();
    int dotPos = str.indexOf('.');
    int fractionLength = dotPos < 0 ? 0 : str.length() - (dotPos + 1);
    String diffString = String(d, fractionLength);

    diffString = removeUnNecessaryDoubleEnding(diffString);

    if (diffString.equals(removeUnNecessaryDoubleEnding(str)))
        return diffString;
    else
        return "";
}

void onShowEditValuePage(DisplayPage *pPage)
{
    DisplayButton *btn;

    //Hide or show dot button
    btn = pPage->getButtonByText(".");

    btn->setState(allowDouble ? VISABLE : HIDDEN);

    //Hide or show minus button
    btn = pPage->getButtonByText("-");
    btn->setState(allowMinus ? VISABLE : HIDDEN);
    DisplayButton *btnValue = pPage->getLastButton();

    btnValue->setText(toString(*btnValue->getLinkedValue()));
}

void onDrawEditValuePage(DisplayPage *pPage)
{
    DisplayButton *btnValue = pPage->getLastButton();
    pPage->getDisplay()->drawString(btnValue->getLinkedValueName(), 12, 44);
    btnValue->draw();
}

void pageEditKeyPressed(DisplayButton *btn)
{
    DisplayPage *pPage = btn->getPage();

    char firstChar = 'x';

    if (btn->getText().length() > 0)
        firstChar = btn->_values.text.charAt(0);
    else
        return; //bad text on button

    DisplayButton *valueButton = btn->getPage()->getLastButton();
    String currentValue = valueButton->getText();
    String newStr = "";
    int currentLength = currentValue.length();
    switch (firstChar)
    {

    case 'O': //OK
        if (valueButton->getLinkedValue())
        {
            newStr = convertToDoubleAndBackToSameString(currentValue);
            double newVal = newStr.toDouble();
            *(valueButton->getLinkedValue()) = newVal;
        }

        pPage->getMenu()->showPage(valueButton->getPageToOpen());
        return;
        break;

    case 'C': //Cancel
        pPage->getMenu()->showPage(valueButton->getPageToOpen());
        return;
        break;

    case 'R': //Reset
        valueButton->setText("0");
        break;

    case '-':
        if (currentValue.startsWith("-"))
        {
            currentValue.remove(0, 1);
        }
        else
        {
            // no negative sign at beginning
            if (currentLength > 0 && currentValue != "0")
            {
                currentValue = String('-') + currentValue;
            }
        }
        valueButton->setText(currentValue);
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (currentValue == "0")
        {
            valueButton->setText(String(firstChar));
        }
        else
        {
            newStr = convertToDoubleAndBackToSameString(currentValue + firstChar);

            if (newStr.length() > 0)
                valueButton->setText(currentValue + firstChar);
        }
        break;

    case '.':
        if (currentValue.indexOf('.') < 0)
        {
            if (currentLength < 0)
                currentValue = '0';

            valueButton->setText(currentValue + firstChar);
        }
        break;

    case 'D': //delete

        if (currentLength > 1)
        {
            currentValue.remove(currentLength - 1, 1);
            valueButton->setText(currentValue);
        }
        else
        {
            valueButton->setText("0");
        }
        break;
    }

    onDrawEditValuePage(pPage);
}

void addPageEditValue(DisplayMenu *pMenu)
{
    DisplayPage *pPage = pMenu->addPage();
    const int rowCount = 4;
    const int colCount = 4;
    const int keyCount = (rowCount * colCount);
    char keys[keyCount][7] = {
        "7", "8", "9", "Delete",
        "4", "5", "6", "Reset",
        "1", "2", "3", "Cancel",
        "0", ".", "-", "OK"};

    uint16_t commandColors[rowCount] = {
        //Delete,  Reset,     Cancel,  OK,
        TFT_BROWN, TFT_BROWN, TFT_RED, TFT_DARKGREEN};

    struct COMMAND_BUTTON
    {
        char text[3];
        uint16_t fillColor;
    };

    const uint16_t TFT_BUTTON_OUTLINE = tft.color565(115, 149, 125),
                   TFT_BUTTON_FILL = tft.color565(48, 73, 47),
                   TFT_BUTTON_TEXT = TFT_GOLD;

    const int buttonWidth = 50,
              buttonHeight = 39,
              buttonMarginX = 10,
              buttonMarginY = 68,
              buttonPaddingX = 5,
              buttonPaddingY = 5,
              buttonCmdWidth = 130,
              buttonCmdMarginX = 180;

    int col, row = 0;
    for (int x = 0; x < keyCount; x++)
    {
        col = x % colCount;
        if (col < (colCount - 1))
        {
            pPage->addFunctionButton(buttonMarginX + (col * (buttonPaddingX + buttonWidth)), buttonMarginY + (row * (buttonHeight + buttonPaddingY)),
                                     buttonWidth, buttonHeight,
                                     TFT_BUTTON_OUTLINE, TFT_BUTTON_FILL, TFT_BUTTON_TEXT, 1, keys[x], pageEditKeyPressed);
        }
        else
        { //Command buttons
            pPage->addFunctionButton(buttonCmdMarginX, buttonMarginY + (row * (buttonHeight + buttonPaddingY)), buttonCmdWidth, buttonHeight, TFT_BUTTON_OUTLINE, commandColors[row], TFT_BUTTON_TEXT, 1, keys[x], pageEditKeyPressed);
        }

        if ((x + 1) % colCount == 0)
            row++;
    }

    //the input display at top of the screen
    DisplayButton *btn = pPage->addFunctionButton(10, 1, 300, buttonHeight, TFT_BUTTON_OUTLINE, pPage->getDisplay()->color565(25, 25, 25), TFT_BUTTON_TEXT, 1, "0", NULL);

    btn->setPageToOpen(pPage->getMenu()->getPage(0));
    btn->setTextAlign(ALIGN_RIGHT, 20, 3);
    pPage->registerOnDrawEvent(onDrawEditValuePage);
    pPage->registerOnShowEvent(onShowEditValuePage);
}
#endif