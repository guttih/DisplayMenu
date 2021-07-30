#ifndef DISPLAYLABEL_H
#define DISPLAYLABEL_H

#include "arduino.h"

#include "FS.h"

#include <SPI.h>

#include <TFT_eSPI.h>

#include "DisplayGlobals.h"

class DisplayLabel;

typedef void (*OnDrawDisplayLabel) (DisplayLabel *ptrLabel);

class DisplayPage;

struct DISPLAY_LABEL_VALUES {
    TFT_eSPI *tft;
    int16_t x;
    int16_t y; 
    int16_t xDatumOffset;
    int16_t yDatumOffset;
    uint16_t width;
    uint16_t height;
    uint16_t outlineColor;
    uint16_t fillColor;
    uint16_t textColor;
    uint8_t textsize;
    uint8_t textDatum;
    uint8_t radius;
    String text;
    DisplayState state;
    DisplayPage *pPage;
    double *pLinkedValue;
    String linkedValueName;
    double incrementValue;
    OnDrawDisplayLabel onDrawDisplayLabel;
}; 

class DisplayLabel
{
private:
    double _dTemp;
    void init(  TFT_eSPI *tft, 
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
                );
public:
    bool  _currentState, 
          _lastState; 
    String getText() { return _values.text; };
    void setText(String newText) { _values.text = newText; };
    void setLinkToValue(double *pLinkedValue, String valueName);
    
    double *getLinkedValue() { return _values.pLinkedValue; };
    String getLinkedValueName() { return _values.linkedValueName; };
    void setDatum(uint8_t textDatum, int16_t xDatumOffset, int16_t yDatumOffset) { _values.textDatum = textDatum; _values.xDatumOffset = xDatumOffset; _values.yDatumOffset = yDatumOffset; };
    void setState(DisplayState state) { _values.state = state; };
    void show() { _values.state = DisplayState::VISABLE; };
    void hide() { _values.state = DisplayState::HIDDEN; };
    DISPLAY_LABEL_VALUES _values;
    DISPLAY_LABEL_VALUES getValues() { return _values; };

    DisplayLabel(const DisplayLabel &label);
    
    DisplayLabel(  TFT_eSPI *tft, 
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
                    );

    DisplayLabel(  TFT_eSPI *tft,
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
                );
    void resetPressState();

    /**
     * @brief draws the button on to the screen
     * 
     * @param inverted Should the button colors be drawn inverted
     * @param checkIfPageIsVisable should the draw be canceled if the page this button belongs to is hidden.
     * Node if you need more speed this variable should be false;
     */
    void draw(bool inverted=false,  bool checkIfPageIsVisable = true);
    void registerOnDrawEvent(OnDrawDisplayLabel pOnDrawDisplayLabel) {
        _values.onDrawDisplayLabel = pOnDrawDisplayLabel;
    }

    void serialPrintValues(unsigned int margin=0);

    DisplayPage *getPage() { return _values.pPage; }
};


#endif