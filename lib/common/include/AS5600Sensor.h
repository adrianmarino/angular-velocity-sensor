#pragma once
#include <Wire.h>

const int ERROR_VALUE = 0xFFFF;
const int DEFAULT_ADDRESS = 0x36; // Dirección I2C del AS5600


class AS5600Sensor
{

private:
    int value;
    int address;

public:
    AS5600Sensor(int address = DEFAULT_ADDRESS): address(address), value(ERROR_VALUE) {}

    bool begin()
    {
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
        Wire.write(0x0E); // Dirección del registro de ángulo (MSB)
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
        Wire.endTransmission(address);

        return value;
    }
};
