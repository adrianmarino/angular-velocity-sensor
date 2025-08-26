#include <MagneticEncoder.h>
#include <I2CMultiplexor.h>
#include <RobotW.h>
#include <DeltaTimeComputer.h>
#include <RobotOdometry.h>
#include <OdometryPublisher.h>
#include <RosNodeManager.h>
#include <RosNodeManagerRestartHandler.h>
#include <Logger.h>
//
//
//
// ----------------------------------------------------------------------------
// Configuration
// ----------------------------------------------------------------------------
// Setear la frecuencia de la CPU a 240 MHz (Máxima)
const uint32_t CpuFreqMhz = 240;

namespace NodeManager
{
    const String Name = "robot_odometry";
    const int CheckConnectionIntervalMs = 10000;
}

namespace I2C
{
    const int SdaPin = 21;
    const int SclPin = 22;
    const uint32_t BusFreq = 400000; // 400kHz para Fast Mode I2C
}

namespace Encoder
{
    const int SampleIntervalMs = 50;
    const float EWWMAAlpha = 0.8;
    const short int Count = 4;
}

namespace Odometry
{
    const unsigned int UpdateIntervalMs = Encoder::SampleIntervalMs;
    const String Topic = "/robot_w";
}

namespace RemoteMicroRosAgent
{
    const String WifiSSID = "RoboNet";
    const String WifiPass = "29042902";
    const String IP = "192.168.1.30";
    const uint16_t Port = 8888;
}

namespace WifiEnergySavingMode
{
    const bool Enable = true;
    const bool Disable = false;
}
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
// Global Variables
// ----------------------------------------------------------------------------
RosNodeManager *nodeManager;

RosNodeManagerRestartHandler *nodeManagerRestartHandler;

RobotW robotW = {0.0, 0.0, 0.0, 0.0};

MagneticEncoder *encoders[Encoder::Count];

RobotOdometry robotOdometry;

DeltaTimeComputer odomPublishTime(Odometry::UpdateIntervalMs);

OdometryPublisher *odometryPublisher;

I2CMultiplexor *multiplexor;

TaskHandle_t rosConnectionTaskHandle = NULL;
// ----------------------------------------------------------------------------
//
//
//