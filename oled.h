#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

class OLED_DISPLAY {
private:
    Adafruit_SH1106G* display;
    bool displayFound;
    
public:
    OLED_DISPLAY();
    bool begin();
    void clear();
    void showWaterLevel(float waterLevel, float percent, float distance, bool wifiConnected, int rssi, String status);
    void showTime(int ngay, int thang, int gio, int phut);
};
#endif