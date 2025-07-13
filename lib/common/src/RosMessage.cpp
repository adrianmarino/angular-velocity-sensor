#include "RosMessage.h"

std_msgs__msg__Float32 *createFloatMessage()
{
  std_msgs__msg__Float32 *msg = new std_msgs__msg__Float32();
  msg->data = 0;
  return msg;
}

std_msgs__msg__Int32 *createIntMessage()
{
  std_msgs__msg__Int32 *msg = new std_msgs__msg__Int32();
  msg->data = 0;
  return msg;
}


std_msgs__msg__Float32MultiArray *createFloatArrayMessage(size_t length) 
{
  std_msgs__msg__Float32MultiArray *msg = new std_msgs__msg__Float32MultiArray();
  msg->data.data = (float *)malloc(sizeof(float) * length);
  if (msg->data.data == NULL) {
    delete msg;
    logger.error("Failed to allocate memory for Float32MultiArray");
    return NULL;
  }
  msg->data.size = length;
  msg->data.capacity = length;
  return msg;
}