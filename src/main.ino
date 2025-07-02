#include <MagneticEncoder.h>

const int I2C_SDA = 21;
const int I2C_SCL = 22;
const int SAMPLE_INTERVAL_MS = 50;
const float EWMA_ALPHA = 0.6;

void onUpdate(int step, float w)
{
    Serial.print(">Step:");
    Serial.print(step);
    Serial.print(",W:");
    Serial.println(w);
}

MagneticEncoder *encoder;
void setup()
{
    Serial.begin(9600);
    while (!Serial && millis() < 5000);

    encoder = new MagneticEncoder(
        I2C_SDA,
        I2C_SCL,
        onUpdate,
        DEFAULT_ADDRESS,
        SAMPLE_INTERVAL_MS,
        EWMA_ALPHA);

    encoder->begin();
}

void loop()
{
    encoder->update();
}