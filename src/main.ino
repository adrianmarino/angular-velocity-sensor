#include <AS5600Sensor.h>
#include <MagneticEncoder.h>
#include <WCalculator.h>

const int I2C_SDA = 21;
const int I2C_SCL = 22;
const int SAMPLE_INTERVAL_MS = 50;
const float EWMA_ALPHA = 0.6;

WCalculator *wCalculator = new WCalculator(EWMA_ALPHA);

void onUpdate(int value,
              float valueDiff,
              float deltaTimeMs)
{
    Serial.print(">w:");
    Serial.println(wCalculator->getWInRadBySec(valueDiff, deltaTimeMs));
}

MagneticEncoder *encoder = new MagneticEncoder(
    new AS5600Sensor(I2C_SDA, I2C_SCL),
    onUpdate,
    SAMPLE_INTERVAL_MS);


void setup()
{
    Serial.begin(9600);
    while (!Serial && millis() < 5000);
    encoder->begin();
}

void loop()
{
    encoder->update();
}