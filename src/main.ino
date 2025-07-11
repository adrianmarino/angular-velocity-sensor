#include "main.h"

void initRosNode()
{
    Serial.println("Start Setup Node...");

    nodeManager = new RosNodeManager(
        NODE_NAME,
        RemoteMicroRosAgent::WifiSSID,
        RemoteMicroRosAgent::WifiPass,
        RemoteMicroRosAgent::IP,
        RemoteMicroRosAgent::Port,
        WifiEnergySavingMode::Enable);

    nodeManager->initWifi();

    syncClockTimeStamp(AR_UTC_TIME_OFFSET_IN_SECONDS);

    nodeManager->setup();
    
    Serial.println("End Setup Node...");
}

void initOdometry(RosNodeManager *nodeManager)
{
    Serial.println("Start Setup Odometry...");

    odometryPublisher = new OdometryPublisher(nodeManager->getNode());

    odomPublishTime.setup();

    Serial.println("End Setup Odometry...");
}

void initEncoders()
{
    Serial.println("Start Setup Encoders...");

    // Inicializa el bus I2C principal del ESP32
    // Los pines SDA y SCL del ESP32 se conectan al multiplexor
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_BUS_FREQ);

    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        Serial.println("");
        Serial.print("Setup Encoder ");
        Serial.print(i);
        Serial.println("...");

        selectI2CMuxChannel(i);
        delay(5);

        encoders[i] = new MagneticEncoder(
            onUpdate,
            i,
            SAMPLE_INTERVAL_MS,
            EWMA_ALPHA);

        encoders[i]->begin();
    }

    Serial.println("End Setup Encoders...");
}

void publishOdometry()
{
    if (odomPublishTime.hasBeenReached())
    {
        robotOdometry.updateFrom(
            robotW.fl,
            robotW.fr,
            robotW.bl,
            robotW.br);
        odometryPublisher->publish(robotOdometry);
        odomPublishTime.reset();
    }
    odomPublishTime.update();
}

void updateEncoders()
{
    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        MagneticEncoder *encoder = encoders[i];
        selectI2CMuxChannel(encoder->getChannel());
        encoder->update();
    }
}

void initSerial(unsigned long baud) {
    Serial.begin(baud);
    while (!Serial && millis() < 5000);
}

void onUpdate(short int id, int step, float w)
{
    if (id == 0)
        robotW.fl = w;
    if (id == 1)
        robotW.fr = w;
    if (id == 2)
        robotW.bl = w;
    if (id == 3)
        robotW.br = w;
}

void setup()
{
    // Setear la frecuencia de la CPU a 240 MHz (Máxima)
    setCpuFrequencyMhz(235);

    initSerial(9600);

    Serial.println("Start Robot Odometry Setup...");

    initEncoders();
    initRosNode();
    initOdometry(nodeManager);

    Serial.println("Finish Robot Odometry Setup...");
}

void loop()
{
    updateEncoders();
    publishOdometry();
}