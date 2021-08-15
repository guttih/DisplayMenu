#include "DisplayButton.h"
#include "DisplayMenu.h"

DisplayButton::DisplayButton(   TFT_eSPI *tft,
                                int16_t x,
                                int16_t y,
                                uint16_t width,
                                uint16_t height,
                                uint16_t outlineColor,
                                uint16_t fillColor,
                                uint16_t textColor,
                                uint8_t textsize, 
                                const char *text,
                                DisplayButtonType type,
                                DisplayPage *page,
                                DisplayPage *pPageToOpen,
                                ButtonPressedFunction buttonPressed
                                )
{

    init(tft, x, y, width, height, outlineColor, fillColor, textColor, textsize, text, type, VISABLE, page, "",  NULL, 0, pPageToOpen, buttonPressed);
}


DisplayButton::DisplayButton(   TFT_eSPI *tft,
                                int16_t x,
                                int16_t y,
                                uint16_t width,
                                uint16_t height,
                                uint16_t outlineColor,
                                uint16_t fillColor,
                                uint16_t textColor,
                                uint8_t textsize, 
                                const char *text,
                                DisplayButtonType type,
                                DisplayPage *page,
                                double *pLinkedValue,
                                double incrementValue
                                )
{

    init(tft, x, y, width, height, outlineColor, fillColor, textColor, textsize, text, type, VISABLE, page, "", pLinkedValue, incrementValue, NULL, NULL);
}

// Copy constructor
DisplayButton::DisplayButton(const DisplayButton &button)
{
    init(button._values.tft           , button._values.x              , button._values.y,
         button._values.width         , button._values.height         , button._values.outlineColor,
         button._values.fillColor     , button._values.textColor      , button._values.textsize,
         button._values.text.c_str()  , button._values.type           , button._values.state,
         button._values.pPage         , button._values.linkedValueName, button._values.pLinkedValue, 
         button._values.incrementValue, button._values.pPageToOpen    , button._values.buttonPressedFunction,
         button._values.textAlign);
}

void DisplayButton::init(   TFT_eSPI *tft, 
                            int16_t x, 
                            int16_t y, 
                            uint16_t width,
                            uint16_t height,
                            uint16_t outlineColor,
                            uint16_t fillColor,
                            uint16_t textColor,
                            uint8_t textsize,
                            const char *text, 
                            DisplayButtonType type,
                            DisplayState state,
                            DisplayPage *page,
                            String linkedValueName,
                            double *pLinkedValue,
                            double incrementValue,
                            DisplayPage *pPageToOpen,
                            ButtonPressedFunction buttonPressedFunction,
                            TextAlign textAlign
                            )
{

    _values.tft = tft;
    _values.x = x;
    _values.y = y;
    _values.width = width;
    _values.height = height;
    _values.outlineColor = outlineColor;
    _values.fillColor = fillColor;
    _values.textColor = textColor;
    _values.textsize = textsize;
    _values.text = text;
    _values.type = type;
    _values.state = state;
    _values.pPage = page;
    _values.linkedValueName = linkedValueName;
    _values.pLinkedValue = pLinkedValue;
    _values.incrementValue = incrementValue;
    _values.pPageToOpen = pPageToOpen;
    _values.buttonPressedFunction = buttonPressedFunction;

    //defaults
    _values.radius = min(width, height) / 6; // Corner radius
    _values.textAlign = textAlign;
    _values.xDatumOffset = 0;
    _values.yDatumOffset = -4;
    _values.onDrawDisplayButton = NULL;
    _values.allowOnlyOneButtonPressedAtATime = type == OPEN_PAGE || type == RUN_FUNCTION? true: false;
}

void DisplayButton::resetPressState () {
    _lastState = false;
  _currentState = false;
}

void DisplayButton::draw(bool inverted, bool cancelDrawIfPageIsNotVisable)
{
    if (_values.state == HIDDEN) {
        return;
    }

    if (cancelDrawIfPageIsNotVisable)
    {
        DisplayPage *pPage = getPage();
        if (pPage)
        {
            DisplayMenu *pMenu = pPage->getMenu();
            if (pMenu && pPage != pMenu->getVisablePage())
                return;
        }
    }

    if (_values.onDrawDisplayButton)
        _values.onDrawDisplayButton(this);

    uint16_t fillColor, outlineColor, textColor;
    if (!inverted)
    {
        fillColor = _values.fillColor;
        outlineColor = _values.outlineColor;
        textColor = _values.textColor;
    }
    else
    {
        fillColor = _values.textColor;
        outlineColor = _values.outlineColor;
        textColor = _values.fillColor;
    }

    int32_t x, xText, y, yText;
    x = xText = _values.x;
    y = yText = _values.y;
    
    //X calc
    int16_t textWidth = _values.tft->textWidth(_values.text);
    if (_values.textAlign == ALIGN_CENTER)
    {
        xText = _values.x + ((_values.width - textWidth) / 2) + _values.xDatumOffset;
    } else if (_values.textAlign == ALIGN_RIGHT)
    {
        xText = x + (_values.width - textWidth) - _values.xDatumOffset;
    } 

    //Y calc
    yText = _values.y + (_values.height / 2) + _values.yDatumOffset;

    uint16_t before_color = _values.tft->textcolor;
    uint8_t  before_textSize = _values.tft->textsize;
    uint8_t  before_textDatum = _values.tft->getTextDatum();
    uint8_t  before_textPadding = _values.tft->getTextPadding();

    _values.tft->setTextColor(textColor);
    _values.tft->setTextSize(_values.textsize);
    //Going to calculate everything from ML
    _values.tft->setTextDatum(ML_DATUM);
    _values.tft->setTextPadding(0);

    _values.tft->fillRoundRect(x, y, _values.width, _values.height, _values.radius, fillColor);
    _values.tft->drawRoundRect(x, y, _values.width, _values.height, _values.radius, outlineColor);

    _values.tft->drawString(_values.text, xText, yText);

    _values.tft->setTextColor(before_color);
    _values.tft->setTextSize(before_textSize);
    _values.tft->setTextDatum(before_textDatum);
    _values.tft->setTextPadding(before_textPadding);

}

bool DisplayButton::contains(int16_t x, int16_t y) {
  return ((x >= _values.x) && (x < (_values.x + _values.width)) &&
          (y >= _values.y) && (y < (_values.y + _values.height)));
}

void DisplayButton::press(bool isPressed) {
  _lastState = _currentState;
  _currentState = isPressed;
}

bool DisplayButton::isPressed()    { 
    return _currentState; 
}
bool DisplayButton::justPressed()  { 
    return (_currentState && !_lastState); 
}
bool DisplayButton::justReleased() { 
    return (!_currentState && _lastState); 
}

bool DisplayButton::executeCommand()
{
    if (_values.state == HIDDEN)
        return false;

    switch (_values.type)
    {
    case RUN_FUNCTION:
        if (_values.buttonPressedFunction)
        {
            _values.buttonPressedFunction(this);
            return true;
        }
        break;

    case OPEN_PAGE:
        if (_values.pPageToOpen)
        {
            DisplayMenu *pMenu = _values.pPageToOpen->getMenu();
            if (pMenu)
                pMenu->showPage(_values.pPageToOpen);
            return true;
        }
        break;

    case INCREMENT_VALUE:
        if (_values.pLinkedValue && _values.incrementValue)
        {

            *_values.pLinkedValue += _values.incrementValue;
            if (_values.pPage)
                _values.pPage->draw(false);
            return true;
        }

        break;
    }
    return false;
}

void DisplayButton::setLinkToValue(double *pLinkedValue, String valueName) { 
    _values.pLinkedValue = pLinkedValue; 
    _values.linkedValueName = valueName; 
};

void DisplayButton::setText(String newText, bool drawScreenNow)
{
    _values.text = newText;
    if (drawScreenNow)
        draw();
};

void DisplayButton::setTextAlign(TextAlign textAlign, int16_t xDatumOffset, int16_t yDatumOffset)
{
    _values.textAlign = textAlign;
    _values.xDatumOffset = xDatumOffset;
    _values.yDatumOffset = yDatumOffset;
};