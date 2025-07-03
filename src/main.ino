#include <MagneticEncoder.h>

const int I2C_SDA = 21;
const int I2C_SCL = 22;
const int SAMPLE_INTERVAL_MS = 50;
const float EWMA_ALPHA = 0.6;
const uint32_t I2C_BUS_FREQ = 400000; // 400kHz para Fast Mode I2C

void onUpdate(int step, float w)
{
    Serial.print(">Step:");
    Serial.print(step);
    Serial.print(",W1:");
    Serial.println(w);
}

MagneticEncoder *encoder;
void setup()
{
    Serial.begin(9600);
    while (!Serial && millis() < 5000);

    // Inicializa el bus I2C principal del ESP32
    // Los pines SDA y SCL del ESP32 se conectan al multiplexor
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_BUS_FREQ);

    encoder = new MagneticEncoder(
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