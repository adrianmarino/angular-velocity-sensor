#pragma once
#include "RobotOdometry.h"
#include "MicroRosPublisher.h"
#include "FloatArrayPublisher.h"

/**
 * OdometryPublisher class for publishing odometry data.
 *
 * This class provides methods to publish odometry data and transform messages
 * using the ROS 2 micro-ROS framework.
 */
class OdometryPublisher
{
private:
    FloatArrayPublisher *publisher;

public:
    /**
     * Constructor for OdometryPublisher.
     * @param node Pointer to the ROS node
     */
    OdometryPublisher(rcl_node_t *node);
    
    /**
     * Publish the odometry data.
     * @param data Robot movement data.
     */
    void publish(const RobotOdometry &data);
};