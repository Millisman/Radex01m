#pragma GCC diagnostic ignored "-Wunknown-attributes"

#include "Arduino/Arduino.h"
#include "Arduino/HardwareSerial.h"
#include "RF24.h"
#include "Radio.h"
#include "../Defines.h"

Radio_Packet_Type pkt;
Radio_Packet_Type Devices[NUM_DEV];

RF24 radio(PIN_A0, 10); //cepin, cspin

uint8_t address[][6] = {"xMain", "xRemo"};

void setup_radio() {
    
    memset(&pkt, 0, sizeof(Radio_Packet_Type));
    
    for (uint8_t i=0; i < NUM_DEV; ++i) {
        memset(&Devices[i], 0, sizeof(Radio_Packet_Type));
    }

    if (!radio.begin()) {
        Serial.println(F("RF24 is not responding!!"));
    } else {
        radio.setPALevel(RF24_PA_LOW);
        radio.setChannel(5);
        radio.setPayloadSize(sizeof(Radio_Packet_Type)); // default value is the maximum 32 bytes
        radio.openWritingPipe(address[0]);      // set the TX address of the RX node into the TX pipe always uses pipe 0
        radio.openReadingPipe(1, address[1]);   // using pipe 1
        // radio.stopListening(); // put radio in TX mode
        radio.startListening();   // put radio in RX mode
        // For debugging info
        radio.printDetails();
    }
}



void dump_Radio_Packet(Radio_Packet_Type *p) {
    Serial.print(F("DevSerNum:")); Serial.println(p->DevSerNum, DEC);
    Serial.print(F("Batt_mV:")); Serial.println(p->Batt_mV, DEC);
    Serial.print(F("Batt_mA:")); Serial.println(p->Batt_mA, DEC);
    Serial.print(F("Solar_mV:")); Serial.println(p->Solar_mV, DEC);
    Serial.print(F("Solar_mA:")); Serial.println(p->Solar_mA, DEC);
    Serial.print(F("Period_AirQ_s:")); Serial.println(p->Period_AirQ_s, DEC);
    Serial.print(F("Period_GM_s:")); Serial.println(p->Period_GM_s, DEC);
    Serial.print(F("Duty_GM_s:")); Serial.println(p->Duty_GM_s, DEC);
    Serial.print(F("Period_Temp_s:")); Serial.println(p->Period_Temp_s, DEC);
    Serial.print(F("Period_Hum_s:")); Serial.println(p->Period_Hum_s, DEC);
    Serial.print(F("Err_Power:")); Serial.println(p->Err_Power, DEC);
    Serial.print(F("Err_HVDC:")); Serial.println(p->Err_HVDC, DEC);
    Serial.print(F("Err_Geiger:")); Serial.println(p->Err_Geiger, DEC);
    Serial.print(F("Err_AirSense:")); Serial.println(p->Err_AirSense, DEC);
    Serial.print(F("Err_TempSense:")); Serial.println(p->Err_TempSense, DEC);
    Serial.print(F("Err_Humidity:")); Serial.println(p->Err_Humidity, DEC);
    Serial.print(F("Err_XReady:")); Serial.println(p->Err_XReady, DEC);
    Serial.print(F("Sen_GM_Cnt_Min:")); Serial.println(p->Sen_GM_Cnt_Min, DEC);
    Serial.print(F("Sen_Air_Q:")); Serial.println(p->Sen_Air_Q, DEC);
    Serial.print(F("Sen_Temp:")); Serial.println(p->Sen_Temp, DEC);
    Serial.print(F("Sen_Humidity:")); Serial.println(p->Sen_Humidity, DEC);
    Serial.print(F("Sen_GM_Zivert:")); Serial.println(p->Sen_GM_Zivert, DEC);
    Serial.println();
    
}



void do_radio() {
    uint8_t pipe;
    if (radio.available(&pipe)) {
        radio.read(&pkt, sizeof(Radio_Packet_Type)); // fetch payload from FIFO
        Serial.print(F("Read pipe:")); Serial.println(pipe, DEC);
//         dump_Radio_Packet(&pkt);

        
        int8_t found_idx = -1;
        for (uint8_t i=0; i < NUM_DEV; ++i) {
            if (pkt.DevSerNum == Devices[i].DevSerNum) { /*|| (Devices[i].DevSerNum == 0)*/
                Serial.print(F("FOUND "));
                found_idx = i;
                break;
            }
        }
        
        if (found_idx != -1) {
            Serial.print(F("UPDATE "));
            memcpy(&Devices[found_idx], &pkt, sizeof(Radio_Packet_Type));
        } else {
            for (uint8_t i=0; i < NUM_DEV; ++i) {
                if (Devices[i].DevSerNum == 0) { /*|| (Devices[i].DevSerNum == 0)*/
                    Serial.print(F("INSERT NEW "));
                    memcpy(&Devices[i], &pkt, sizeof(Radio_Packet_Type));
                    //break;
                    break;
                }
            }
        }
        
//         for (uint8_t i=0; i < NUM_DEV; ++i) {
//             if (pkt.DevSerNum == Devices[i].DevSerNum) { /*|| (Devices[i].DevSerNum == 0)*/
//                 found_idx = i;
//                 //                 memcpy(&Devices[i], &pkt, sizeof(Radio_Packet_Type));
//                 break;
//             }
//         }
    }
}
