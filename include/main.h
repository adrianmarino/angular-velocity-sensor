#include <MagneticEncoder.h>
#include <I2CMultiplexor.h>
#include <RobotW.h>
#include <DeltaTimeComputer.h>
#include <RobotOdometry.h>
#include <OdometryPublisher.h>
#include <RosNodeManager.h>
#include <Logger.h>

const String NODE_NAME = "robot_odometry";

const int I2C_SDA = 21;

const int I2C_SCL = 22;

const int SAMPLE_INTERVAL_MS = 50;

const float EWMA_ALPHA = 0.8;

const uint32_t I2C_BUS_FREQ = 400000; // 400kHz para Fast Mode I2C

const short int ENCODERS_COUNT = 4;

const unsigned int UPDATE_INTERVAL_MS = 50;

const String ODOMETRYY_TOPIC = "/robot_w";

namespace RemoteMicroRosAgent
{
    const String WifiSSID = "Zion24";
    const String WifiPass = "lv3jg62904";
    const String IP = "192.168.2.116";
    const uint16_t Port = 8888;
}

namespace WifiEnergySavingMode
{
    const bool Enable = true;
    const bool Disable = false;
}

RosNodeManager *nodeManager;

RobotW robotW = {0.0, 0.0, 0.0, 0.0};

MagneticEncoder *encoders[ENCODERS_COUNT];

RobotOdometry robotOdometry;

DeltaTimeComputer odomPublishTime(UPDATE_INTERVAL_MS);

OdometryPublisher *odometryPublisher;

I2CMultiplexor *multiplexor;