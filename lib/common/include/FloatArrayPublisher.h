#pragma once
#include "RosMessage.h"
#include "MicroRosPublisher.h"

/**
 * FloatArrayPublisher class for publishing float arrays using micro-ROS.
 *
 * This class provides methods to create a publisher and publish float arrays
 * using the micro-ROS framework.
 */
class FloatArrayPublisher {
public:
    /**
     * Constructor for FloatArrayPublisher.
     * @param publisher Pointer to the MicroRosPublisher object
     * @param length Length of the float array
     */
    FloatArrayPublisher(MicroRosPublisher* publisher, size_t length);

    ~FloatArrayPublisher();

    /** 
     * Publish the float array.
     * @param data_array Pointer to the float array to be published.
     */
    void publish(float *data_array);

private:
    MicroRosPublisher *publisher;
    std_msgs__msg__Float32MultiArray *msg;
    size_t length;
};