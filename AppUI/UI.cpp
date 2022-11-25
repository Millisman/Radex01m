#pragma GCC diagnostic ignored "-Wunknown-attributes"

#include "UI.h"
#include "LiquidCrystal.h"
#include "../Defines.h"
#include "AppRadio/Radio.h"

#define REDRAW_UI   2000

extern Radio_Packet_Type Devices[NUM_DEV];

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

static FILE file_lcd0;

int putc_lcd0(char ch, [[gnu::unused]] FILE *stream) {
    lcd.write(ch);
    return 0;
}

void setup_UI() {
    lcd.begin(20, 4);
    lcd.clear();
    fdev_setup_stream( &file_lcd0, putc_lcd0,   NULL, _FDEV_SETUP_WRITE);
    stderr = &file_lcd0;
    // TODO add Logo
}

uint32_t time_lcd_redraw = 0;
uint8_t  lcd_page = 0;
uint8_t  lcd_page_count = 3;

void do_UI() {
    if((millis() - time_lcd_redraw) > REDRAW_UI) {
        time_lcd_redraw = millis();
        lcd.clear();
        uint8_t line = 0;
        for (uint8_t i=0; i < NUM_DEV; ++i) {
            if (Devices[i].DevSerNum) {
                lcd.setCursor(0, line);
                ++line;
                
                switch (lcd_page) {
                    case 0: {
                        fprintf_P(stderr, PSTR("%5u AIR: "), Devices[i].DevSerNum);
                        switch (Devices[i].Sen_Air_Q) {
                            case 0: fprintf_P(stderr, PSTR("VERY BAD!")); break;
                            case 1: fprintf_P(stderr, PSTR("BAD! BAD!")); break;
                            case 2: fprintf_P(stderr, PSTR("PASSABLY!")); break;
                            case 3: fprintf_P(stderr, PSTR("FRESH :) ")); break;
                        }
                    }
                    break;
                        
                    case 1: {
                        fprintf_P(stderr, PSTR("Zivert %2.3f/%4u m"), Devices[i].Sen_GM_Zivert, Devices[i].Sen_GM_Cnt_Min);
                    }
                    break;
                            
                            
                    case 2: {
                        fprintf_P(stderr, PSTR("Tmp %3.1f Hum %3.1f %"), Devices[i].Sen_Temp, Devices[i].Sen_Humidity);
                    }
                    break;
                        
                    case 3: {
                        fprintf_P(stderr, PSTR("ERR %u %u %u %u %u %u %u"),
                            Devices[i].Err_Power,
                            Devices[i].Err_HVDC,
                            Devices[i].Err_Geiger,
                            Devices[i].Err_AirSense,
                            Devices[i].Err_TempSense,
                            Devices[i].Err_Humidity,
                            Devices[i].Err_XReady
                        );
                    }
                    break;
                }
            }

        }
        --lcd_page_count;
        
        if (lcd_page_count == 0) {
            ++lcd_page;
            if (lcd_page > 3) { lcd_page = 0; }
            lcd_page_count = 3;
        }
    }
}
