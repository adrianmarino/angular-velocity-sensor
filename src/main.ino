#include "main.h"

void initRosNode()
{
    logger.info("Start Setup Node...");

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
    
    logger.info("End Setup Node...");
}

void initOdometry(RosNodeManager *nodeManager)
{
    logger.info("Start Setup Odometry...");

    odometryPublisher = new OdometryPublisher(
        nodeManager->getNode(), 
        ODOMETRYY_TOPIC);

    odomPublishTime.setup();

    logger.info("End Setup Odometry...");
}

void initEncoders()
{
    logger.info("Start Setup Encoders...");

    // Inicializa el bus I2C principal del ESP32
    // Los pines SDA y SCL del ESP32 se conectan al multiplexor
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(I2C_BUS_FREQ);

    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        logger.info("Setup Encoder " + String(i) + "...");

        selectI2CMuxChannel(i);
        delay(5);

        encoders[i] = new MagneticEncoder(
            onUpdate,
            i,
            SAMPLE_INTERVAL_MS,
            EWMA_ALPHA);

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
    for (int i = 0; i < ENCODERS_COUNT; i++)
    {
        MagneticEncoder *encoder = encoders[i];
        selectI2CMuxChannel(encoder->getChannel());
        encoder->update();
    }
}


void onUpdate(short int id, int step, float w)
{
    switch (id) {
    case 0:
        robotW.fl = w;
        // logger.debugPlot("w_fl", robotW.fl);
        break;
    case 1:
        robotW.fr = w;
        // logger.debugPlot("w_fr", robotW.fr);
        break;
    case 2:
        robotW.bl = w;
        // logger.debugPlot("w_bl", robotW.bl);
        break;
    case 3:
        robotW.br = w;
        // logger.debugPlot("w_br", robotW.br);
        break;
    }
}

void setup()
{
    // Setear la frecuencia de la CPU a 240 MHz (Máxima)
    setCpuFrequencyMhz(235);

    sleep(5);

    logger.info("Start Robot Odometry Setup...");

    initEncoders();
    initRosNode();
    initOdometry(nodeManager);

    logger.info("Finish Robot Odometry Setup...");
    logger.info("Publishing Robot Odometry...");
}

void loop()
{
    // Process incoming ROS messages and call callbacks
    nodeManager->update();

    updateEncoders();

    publishOdometry();
}