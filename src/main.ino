#include <AS5600Sensor.h>
#include <MagneticEncoder.h>
#include <AngularVelocityCalculator.h>

MagneticEncoder *encoder;
AngularVelocityCalculator *angularVelocityCalculator;

void onValueChange(int Value,
                   float valueDiff,
                   float deltaTimeMs)
{
    float w = angularVelocityCalculator->perform(valueDiff, deltaTimeMs);

    Serial.print("Grad: ");
    Serial.print(Value);
    Serial.print(" | W: ");
    Serial.println(w);
}

void setup()
{
    Serial.begin(9600);
    while (!Serial && millis() < 5000);

    encoder = new MagneticEncoder(
        new AS5600Sensor(
            21,
            22, // Pines 21 y 22 para I2C
            DEFAULT_ADRESSS,
            DEFAULT_I2C_FREQ),
        onValueChange);

    if (!encoder->begin())
    {
        while(true) {}
    }

    angularVelocityCalculator = new AngularVelocityCalculator();
}

void loop() {
    encoder->update();
}