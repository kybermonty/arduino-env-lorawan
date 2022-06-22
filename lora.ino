#include <MKRWAN.h>
#include <Arduino_MKRENV.h>
#include "ArduinoLowPower.h"

String appEui = "XXX";
String appKey = "XXX";

LoRaModem modem;
float temperature;
float humidity;
float pressure;
float illuminance;

void measure() {
    temperature = ENV.readTemperature();
    humidity    = ENV.readHumidity();
    pressure    = ENV.readPressure(MILLIBAR);
    illuminance = ENV.readIlluminance();
}

void send() {
    static uint8_t buffer[7];
    memset(buffer, 0xff, sizeof(buffer));

    int16_t temperature_i16 = (int16_t) (temperature * 10.f);
    buffer[0] = temperature_i16 >> 8;
    buffer[1] = temperature_i16;

    buffer[2] = humidity * 2;

    uint16_t pressure_i16 = pressure / 2.f;
    buffer[3] = pressure_i16 >> 8;
    buffer[4] = pressure_i16;

    if (illuminance > 65534) {
        illuminance = 65534;
    }
    uint16_t illuminance_i16 = (uint16_t) illuminance;
    buffer[5] = illuminance_i16 >> 8;
    buffer[6] = illuminance_i16;

    modem.beginPacket();
    modem.write(buffer, sizeof(buffer));
    modem.endPacket();
    modem.sleep();
}

void setup() {
    if (!ENV.begin()) {
        while (1);
    }
    if (!modem.begin(EU868)) {
        while (1);
    };
    modem.configureClass(CLASS_A);

    int connected = modem.joinOTAA(appEui, appKey);
    if (!connected) {
        while (1);
    }
}

void loop() {
    measure();
    send();

    LowPower.deepSleep(5 * 60 * 1000);
}
