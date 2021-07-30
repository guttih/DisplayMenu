# DisplayMenu

This library helps with creating menu pages when using graphical displays supported by the [TFT_eSPI] library.

## To use this library


### First
 - If you use the __Arduino IDE__, after installing the library [TFT_eSPI] you will need to change the file `User_Setup.h`.  See here for an excellent [tutorial].
 - If you use the __Platform io__, you do not need to change the file `User_Setup.h` but instead you will need to add these build settings to the project `platformio.ini` file in your project folder.
 ```
    build_flags =
    ;###############################################################
    ; TFT_eSPI library setting here (no need to edit library files):
    ;###############################################################
    -Os
    -DCORE_DEBUG_LEVEL=ARDUHAL_LOG_LEVEL_DEBUG
    -DUSER_SETUP_LOADED=1 ; Set this settings as valid
    -DILI9341_DRIVER=1    ; Select ILI9163 driver, a generic driver for common displays
    -DTFT_MISO=19
    -DTFT_MOSI=23
    -DTFT_SCLK=18
    -DTFT_CS=15    ; Chip select control pin
    -DTFT_DC=2     ; Data Command control pin
    -DTFT_RST=4    ; Reset pin (could connect to RST pin)
    -DTOUCH_CS=21  ; Chip select pin (T_CS) of touch screen
    -DLOAD_GLCD=1  ; Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
    -DLOAD_FONT2=1 ; Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
    -DLOAD_FONT4=1 ; Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
    -DLOAD_FONT6=1 ; Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only character    1234567890:-.apm
    -DLOAD_FONT7=1 ; Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only character    1234567890:-.
    -DLOAD_FONT8=1 ; Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only character    1234567890:-.
    -DLOAD_GFXFF=1 ; FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 an    custom fonts
    -DSMOOTH_FONT=1
    -DSPI_FREQUENCY=27000000
    -DSPI_READ_FREQUENCY=20000000
    -DSPI_TOUCH_FREQUENCY=2500000
```
  ### Next
  Then you can just add
 ```
     #include <DisplayMenu.h>
 ```
 and you'r good to go


[TFT_eSPI]: https://github.com/Bodmer/TFT_eSPI
[tutorial]: https://www.xtronical.com/esp32ili9341/