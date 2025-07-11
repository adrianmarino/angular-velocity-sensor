#pragma once
#include <Arduino.h>

const int MUX_ADDRESS = 0x70;

void selectI2CMuxChannel(uint8_t channel)
{
    if (channel > 7)
        return;
    Wire.beginTransmission(MUX_ADDRESS);
    Wire.write(1 << channel);
    Wire.endTransmission();
}