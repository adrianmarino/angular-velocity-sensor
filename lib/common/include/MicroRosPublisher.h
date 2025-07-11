#pragma once
#include "RosUtils.h"
#include "RosMessage.h"
#include <StringUtils.h>

/**
 * MicroRosPublisher class for publishing messages using micro-ROS.
 *
 * This class provides methods to create a publisher and publish messages
 * using the micro-ROS framework.
 */
class MicroRosPublisher
{
private:
  rcl_publisher_t publisher;
  int delayMillis;

public:
  /**
   * Constructor for MicroRosPublisher.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param type_support Type support for the message
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createInt(
      rcl_node_t *node,
      String topic_name,
      int delayMillis = 0,
      bool reliable = true);

  /**
   * Create a publisher for float arrays.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createFloat(
      rcl_node_t *node,
      String topic_name,
      int delayMillis = 0,
      bool reliable = true);

  /**
   * Create a publisher for float arrays.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param delayMillis Delay in milliseconds between messages
   */
  static MicroRosPublisher *createFloatArray(
      rcl_node_t *node,
      String topic_name,
      int delayMillis = 0,
      bool reliable = true);

  /**
   * Create a publisher for string messages.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param delayMillis Delay in milliseconds between messages (optional)
   */
  static MicroRosPublisher *createString(
      rcl_node_t *node,
      String topic_name,
      int delayMillis = 0,
      bool reliable = true);
  
  /**
   * Create a publisher for messages.
   * @param node Pointer to the ROS node
   * @param topic_name Name of the topic to publish
   * @param type_support Type support for the message
   * @param delayMillis Delay in milliseconds between messages
   * @param reliable True for rclc_publisher_init_default, false for rclc_publisher_init_best_effort
   */
  MicroRosPublisher(
      rcl_node_t *node,
      String topic_name,
      const rosidl_message_type_support_t *type_support,
      int delayMillis = 0,
      bool reliable = true);

  /**
   * Publish the message.
   * @param message Pointer to the message to be published
   */
  void publish(const void *message);
};