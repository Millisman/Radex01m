#include "Arduino/Arduino.h"
#include "AppRadio/Radio.h"
#include "AppUI/LiquidCrystal.h"
#include "AppUI/UI.h"
#include "AppRadio/printf.h"



void setup(void) {
    Serial.begin(115200);
    printf_begin();
    Serial.println(F("\n\rRadex Master"));
    //-----------------
    setup_UI();
    setup_radio();
}

void loop(void) {
    do_radio();
    do_UI();
    // do sleep
    // TODO timer2 - sleep
}
