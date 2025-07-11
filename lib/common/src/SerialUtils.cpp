#include "SerialUtils.h"

void initSerial(unsigned long baud)
{
    Serial.begin(baud);
    while (!Serial && millis() < 5000)
        ;
}

void info(String msg)
{
    Serial.println("INFO: " + String(msg));
}

void error(String msg)
{
    Serial.println("ERROR: " + String(msg));
}

void warn(String msg)
{
    Serial.println("WARN: " + String(msg));
}
