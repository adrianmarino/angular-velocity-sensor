#pragma once
#include <Wire.h>

const int ERROR_VALUE = 0xFFFF;
const int DEFAULT_ADDRESS = 0x36;         // Dirección I2C del AS5600
const uint32_t DEFAULT_I2C_FREQ = 400000; // 400kHz para Fast Mode I2C

class AS5600Sensor
{

private:
    int sdaPin;
    int sclPin;
    int value;
    const int address;
    uint32_t i2cFreq;

public:
    AS5600Sensor(
        int sdaPin,
        int sclPin,
        int address = DEFAULT_ADDRESS,
        uint32_t i2cFreq = DEFAULT_I2C_FREQ) : sdaPin(sdaPin),
                                               sclPin(sclPin),
                                               address(address),
                                               i2cFreq(i2cFreq),
                                               value(ERROR_VALUE) {}

    bool begin()
    {
        Wire.begin(sdaPin, sclPin);
        Wire.setClock(i2cFreq);

        update();
        return isSuccessful();
    }

    bool isSuccessful() const
    {
        return value != ERROR_VALUE;
    }

    int getValue()
    {
        return value;
    }

    float update()
    {
        Wire.beginTransmission(address);
        Wire.write(0x0E);                         // Dirección del registro de ángulo (MSB)
        byte error = Wire.endTransmission(false); // No liberar el bus I2C

        if (error != 0)
        {                        // Si hay un error en la transmisión I2C
            value = ERROR_VALUE; // Marcar como error
            return value;
        }

        Wire.requestFrom(address, 2); // Solicitar 2 bytes (MSB y LSB)

        if (Wire.available() == 2)
        {
            value = Wire.read() << 8; // MSB (bits 11-8)
            value |= Wire.read();     // LSB (bits 7-0)
        }
        else
        {
            value = ERROR_VALUE;
        }

        return value;
    }
};
