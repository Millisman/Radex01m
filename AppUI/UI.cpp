#include "UI.h"
#include "LiquidCrystal.h"
#include "../Defines.h"
#include "AppRadio/Radio.h"

extern Radio_Packet_Type Devices[NUM_DEV];

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

void setup_UI() {
    lcd.begin(20, 4);
    // TODO add Logo
}

void do_UI() {
    uint8_t line = 0;
    for (uint8_t i=0; i < NUM_DEV; ++i) {
        if (Devices[i].DevSerNum) {
            lcd.setCursor(0, line);
            ++line;
            lcd.print(Devices[i].DevSerNum, DEC);
            lcd.print(" ");
            lcd.print(Devices[i].Batt_mV, DEC);
            lcd.print("mV ");
        }
    }
}
