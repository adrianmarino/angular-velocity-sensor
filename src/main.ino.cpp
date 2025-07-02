# 1 "/var/tmp/tmpnl7hmqk2"
#include <Arduino.h>
# 1 "/home/adrian/development/angular-velocity-sensor/src/main.ino"
#include <AS5600Sensor.h>
#include <MagneticEncoder.h>
#include <WCalculator.h>

const int SDA_PIN = 20;
const int SCL_PIN = 21;
const int SAMPLE_INTERVAL_MS = 50;
const int FILTER_ALPHA = 0.6;

WCalculator *wCalculator = new WCalculator(FILTER_ALPHA);
void readValue(int value,
                   float valueDiff,
                   float deltaTimeMs);
void setup();
void loop();
#line 12 "/home/adrian/development/angular-velocity-sensor/src/main.ino"
void readValue(int value,
                   float valueDiff,
                   float deltaTimeMs)
{
    Serial.print(">w:");
    Serial.println(wCalculator->getWInRadBySec(valueDiff, deltaTimeMs));
}

AS5600Sensor *sensor = new AS5600Sensor(
    SDA_PIN,
    SCL_PIN,
    DEFAULT_ADDRESS);

MagneticEncoder *encoder = new MagneticEncoder(
    sensor,
    readValue,
    SAMPLE_INTERVAL_MS);

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 5000)
        ;

    encoder->begin();
}

void loop()
{
    encoder->update();
}