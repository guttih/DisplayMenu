#include "DisplayLabel.h"
#include "DisplayMenu.h"

DisplayLabel::DisplayLabel(   TFT_eSPI *tft,
                                int16_t x,
                                int16_t y,
                                uint16_t width,
                                uint16_t height,
                                uint16_t outlineColor,
                                uint16_t fillColor,
                                uint16_t textColor,
                                uint8_t textsize, 
                                const char *text,
                                DisplayPage *page
                                )
{

    init(tft, x, y, width, height, outlineColor, fillColor, textColor, textsize, text, VISABLE, page, "", NULL, 0);
}


DisplayLabel::DisplayLabel(   TFT_eSPI *tft,
                                int16_t x,
                                int16_t y,
                                uint16_t width,
                                uint16_t height,
                                uint16_t outlineColor,
                                uint16_t fillColor,
                                uint16_t textColor,
                                uint8_t textsize, 
                                const char *text,
                                DisplayPage *page,
                                double *pLinkedValue,
                                double incrementValue
                                )
{

    init(tft, x, y, width, height, outlineColor, fillColor, textColor, textsize, text,  VISABLE, page, "", pLinkedValue, incrementValue);
}

// Copy constructor
DisplayLabel::DisplayLabel(const DisplayLabel &label)
{
    init(label._values.tft           , label._values.x              , label._values.y,
         label._values.width         , label._values.height         , label._values.outlineColor,
         label._values.fillColor     , label._values.textColor      , label._values.textsize,
         label._values.text.c_str()  , label._values.state          , label._values.pPage,
         label._values.linkedValueName, label._values.pLinkedValue  , label._values.incrementValue);
}

void DisplayLabel::serialPrintValues(unsigned int margin)
{
    for(int i = 0; i<margin; i++) 
        Serial.print(" "); 

    Serial.print("x:");              Serial.print(_values.x);
    Serial.print(", y:");            Serial.print(_values.y);
    Serial.print(", width:");        Serial.print(_values.width);
        Serial.print(", height:");   Serial.print(_values.height);
    Serial.print(", outlineColor:"); Serial.print(_values.outlineColor);   
    Serial.print(", fillColor:");    Serial.print(_values.fillColor, HEX); 
    Serial.print(", textColor:");    Serial.print(_values.textColor, HEX);
    Serial.print(", text:");         Serial.print(_values.text);
    Serial.print(", textsize:");     Serial.print(_values.textsize);
    Serial.print(", radius:");       Serial.print(_values.radius);
    Serial.print(", textDatum:");    Serial.print(_values.textDatum);
    Serial.print(",tft:");           Serial.print((unsigned long)_values.tft, HEX);
    Serial.println();
}

void DisplayLabel::init(   TFT_eSPI *tft, 
                            int16_t x, 
                            int16_t y, 
                            uint16_t width,
                            uint16_t height,
                            uint16_t outlineColor,
                            uint16_t fillColor,
                            uint16_t textColor,
                            uint8_t textsize,
                            const char *text, 
                            DisplayState state,
                            DisplayPage *page,
                            String linkedValueName,
                            double *pLinkedValue,
                            double incrementValue
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
    _values.state = state;
    _values.pPage = page;
    _values.linkedValueName = linkedValueName;
    _values.pLinkedValue = pLinkedValue;
    _values.incrementValue = incrementValue;

    //defaults
    _values.radius = min(width, height) / 6; // Corner radius
    _values.textDatum = MC_DATUM;
    _values.xDatumOffset = 0;
    _values.yDatumOffset = 2;
    _values.onDrawDisplayLabel = NULL;
}

void DisplayLabel::resetPressState () {
    _lastState = false;
  _currentState = false;
}

void DisplayLabel::draw(bool inverted, bool cancelDrawIfPageIsNotVisable)
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

    if (_values.onDrawDisplayLabel)
        _values.onDrawDisplayLabel(this);

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

    _values.tft->fillRoundRect(_values.x, _values.y, _values.width, _values.height, _values.radius, fillColor);
    _values.tft->drawRoundRect(_values.x, _values.y, _values.width, _values.height, _values.radius, outlineColor);

    uint16_t before_color = _values.tft->textcolor;
    uint8_t  before_textSize = _values.tft->textsize;
    uint8_t  before_textDatum = _values.tft->getTextDatum();
    uint8_t  before_textPadding = _values.tft->getTextPadding();

    _values.tft->setTextColor(textColor);
    _values.tft->setTextSize(_values.textsize);
    _values.tft->setTextDatum(_values.textDatum);
    _values.tft->setTextPadding(0);

    _values.tft->drawString(_values.text, _values.x + (_values.width/2) + _values.xDatumOffset, _values.y + (_values.height/2) - 4 + _values.yDatumOffset);

    _values.tft->setTextColor(before_color);
    _values.tft->setTextSize(before_textSize);
    _values.tft->setTextDatum(before_textDatum);
    _values.tft->setTextPadding(before_textPadding);

}

void DisplayLabel::setLinkToValue(double *pLinkedValue, String valueName) { 
    _values.pLinkedValue = pLinkedValue; 
    _values.linkedValueName = valueName; 
};

void DisplayLabel::setText(String newText, bool drawScreenNow = false)
{
    _values.text = newText;
    if (drawScreenNow)
        draw();
};