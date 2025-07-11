#include "OdometryPublisher.h"

OdometryPublisher::OdometryPublisher(rcl_node_t *node)
{
    publisher = new FloatArrayPublisher(MicroRosPublisher::createFloatArray(node, "/robot_w"), 2);
}

void OdometryPublisher::publish(const RobotOdometry &data)
{
    float w_data[] = {data.getLeftWInRad(), data.getRightWInRad()};

    publisher->publish(w_data);
}

OdometryPublisher::~OdometryPublisher() {
    delete publisher;
}