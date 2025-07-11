#include <MagneticEncoder.h>

const int I2C_SDA = 21;
const int I2C_SCL = 22;
const int SAMPLE_INTERVAL_MS = 20;
const float EWMA_ALPHA = 0.8;
const uint32_t I2C_BUS_FREQ = 400000; // 400kHz para Fast Mode I2C
const short int ENCODERS_COUNT = 4;
const int MUX_ADDRESS = 0x70;

MagneticEncoder *encoders[ENCODERS_COUNT];

void selectI2CChannel(uint8_t channel)
{
    if (channel > 7)
        return;
    Wire.beginTransmission(MUX_ADDRESS);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

void onUpdate(short int id, int step, float w)
{
    Serial.print(">");
    Serial.print(id);
    Serial.print(":");
    Serial.println(w);
}

void setup()
{
    // Setear la frecuencia de la CPU a 240 MHz (Máxima)
    setCpuFrequencyMhz(235);

    Serial.begin(9600);
    while (!Serial && millis() < 5000)
        ;

    // Inicializa el bus I2C principal del ESP32
    // Los pines SDA y SCL del ESP32 se conectan al multiplexor
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_BUS_FREQ);

    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        Serial.print("Setup Encoder ");
        Serial.print(i);
        Serial.println("...");

        selectI2CChannel(i);
        delay(5);

        encoders[i] = new MagneticEncoder(
            onUpdate,
            i,
            SAMPLE_INTERVAL_MS,
            EWMA_ALPHA);

        encoders[i]->begin();
    }
}

void loop()
{
    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        MagneticEncoder *encoder = encoders[i];
        selectI2CChannel(encoder->getChannel());
        encoder->update();
    }
}
