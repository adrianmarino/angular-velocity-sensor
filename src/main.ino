#include "main.h"

void initRosNode()
{
    logger.info("Setup Node manager...");

    nodeManager = (new RosNodeManager(
                       NodeManager::Name,
                       RemoteMicroRosAgent::WifiSSID,
                       RemoteMicroRosAgent::WifiPass,
                       RemoteMicroRosAgent::IP,
                       RemoteMicroRosAgent::Port,
                       WifiEnergySavingMode::Disable,
                       WIFI_POWER_20_5dBm,
                       false))
                      ->setup();

    nodeManagerRestartHandler = new RosNodeManagerRestartHandler(nodeManager);

    logger.info("End Setup Node...");
}

void initOdometry(rcl_node_t *node)
{
    logger.info("Start Setup Odometry...");

    odometryPublisher = new OdometryPublisher(
        node,
        Odometry::Topic);

    odomPublishTime.setup();

    logger.info("End Setup Odometry...");
}

void initEncoders()
{
    logger.info("Start Setup Encoders...");

    // Inicializa el bus I2C principal del ESP32
    // Los pines SDA y SCL del ESP32 se conectan al multiplexor
    Wire.begin(I2C::SdaPin, I2C::SclPin);
    Wire.setClock(I2C::BusFreq);

    multiplexor = new I2CMultiplexor();

    for (int i = 0; i < Encoder::Count; i++)
    {
        logger.info("Setup Encoder " + String(i) + "...");

        multiplexor->selectChannel(i);
        delay(5);

        encoders[i] = new MagneticEncoder(
            onUpdate,
            i,
            Encoder::SampleIntervalMs,
            Encoder::EWWMAAlpha);

        encoders[i]->begin();
    }

    logger.info("End Setup Encoders...");
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
    for (int i = 0; i < Encoder::Count; i++)
    {
        MagneticEncoder *encoder = encoders[i];
        multiplexor->selectChannel(encoder->getChannel());
        encoder->update();
    }
}

void onUpdate(short int id, int step, float w)
{
    switch (id)
    {
    case 0:
        robotW.fr = w;
        logger.debugPlot("w_fr", robotW.fr);
        break;
    case 1:
        robotW.br = w;
        logger.debugPlot("w_br", robotW.br);
        break;
    case 2:
        robotW.bl = -w;
        logger.debugPlot("w_bl", robotW.bl);
        break;
    case 3:
        robotW.fl = -w;
        logger.debugPlot("w_fl", robotW.fl);
        break;
    }
}

void setup()
{
    setCpuFrequencyMhz(CpuFreqMhz);

    // logger.setLevel(DEBUG);

    logger.info("Start Robot Odometry Setup...");

    initEncoders();
    initRosNode();
    initOdometry(nodeManager->getNode());

    logger.info("Finish Robot Odometry Setup...");
    logger.info("Publishing Robot Odometry...");
}

void loop()
{
    nodeManagerRestartHandler->update();

    updateEncoders();

    publishOdometry();
}