#pragma once
#include <RosUtils.h>
#include <StringUtils.h>
#include <Logger.h>
#include <rclc/executor.h>
#include <timestamp.h>

/**
 * @brief Class for managing a ROS node.
 *
 * This class provides methods to initialize, set up, and update a ROS node
 * using the micro-ROS framework. It also manages the executor and allocator
 * for the node.
 */
class RosNodeManager
{
private:
    String nodeName;
    String wifi_ssid;
    String wifi_pass;
    String agent_ip;
    uint16_t agent_port;
    rclc_support_t support;
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rcl_node_t node;
    rclc_executor_t executor;
    bool wifiEnergySavingMode;
    wifi_power_t wifi_power;

public:
    /**
     * @brief Constructor for RosNodeManager.
     *
     * @param nodeName Name of the ROS node.
     * @param wifi_ssid Wi-Fi SSID for connection.
     * @param wifi_pass Wi-Fi password for connection.
     * @param agent_ip IP address of the micro-ROS agent.
     * @param agent_port Port number of the micro-ROS agent (default is 8888).
     */
    RosNodeManager(
        String nodeName,
        String wifi_ssid,
        String wifi_pass,
        String agent_ip,
        uint16_t agent_port = 8888,
        bool wifiEnergySavingMode = false,
        wifi_power_t wifi_power = WIFI_POWER_20_5dBm
    );

    void initWifi();
    void setup();

    void update(const uint64_t timeout_ns = RCL_MS_TO_NS(2));

    rclc_support_t *getSupport();

    rcl_node_t *getNode();

    rcl_allocator_t *getAllocator();

    rclc_executor_t *getExecutor();
};