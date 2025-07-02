#include <AS5600Sensor.h>
#include <MagneticEncoder.h>
#include <AngularVelocityCalculator.h>

MagneticEncoder *encoder;
AngularVelocityCalculator *filteredVelocity;

void onValueChange(int value,
                  float valueDiff,
                  float deltaTimeMs)
{
    float filterValue = filteredVelocity->perform(valueDiff, deltaTimeMs);
    Serial.print(">w:");
    Serial.println(filterValue);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 5000);

    encoder = new MagneticEncoder(
        new AS5600Sensor(
            21,
            22, // Pines 21 y 22 para I2C
            DEFAULT_ADRESSS),
        onValueChange,
        50);

    if (!encoder->begin())
    {
        while(true) {}
    }

    filteredVelocity = new AngularVelocityCalculator(0.6);
}

void loop()
{
    encoder->update();
}