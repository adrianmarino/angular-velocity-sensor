#pragma once
#include "RosUtils.h"
#include "Logger.h"
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float32_multi_array.h>
#include <std_msgs/msg/string.h>

/**
 * @brief Create a Float32 message.
 * @return Pointer to the created Float32 message.
 */
std_msgs__msg__Float32 *createFloatMessage();

/**
 * @brief Create an Int32 message.
 * @return Pointer to the created Int32 message.
 */
std_msgs__msg__Int32 *createIntMessage();

/**
 * @brief Create a Float32MultiArray message.
 * @param length Length of the array.
 * @return Pointer to the created Float32MultiArray message.
 */
std_msgs__msg__Float32MultiArray *createFloatArrayMessage(size_t length);