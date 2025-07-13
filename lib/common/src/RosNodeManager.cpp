#include <RosNodeManager.h>

RosNodeManager::RosNodeManager(
    String nodeName,
    String wifi_ssid,
    String wifi_pass,
    String agent_ip,
    uint16_t agent_port,
    bool wifiEnergySavingMode,
    wifi_power_t wifi_power
) {
    this->nodeName = nodeName;
    this->wifi_ssid = wifi_ssid;
    this->wifi_pass = wifi_pass;
    this->agent_ip = agent_ip;
    this->agent_port = agent_port;
    this->wifiEnergySavingMode = wifiEnergySavingMode;
    this->wifi_power = wifi_power;
}

void RosNodeManager::initWifi()
{
    logger.info("Wait for wifi connection...");
    connect_to_agent_via_wifi(
        wifi_ssid,
        wifi_pass,
        agent_ip,
        agent_port,
        wifiEnergySavingMode,
        wifi_power
    );
}

void RosNodeManager::setup()
{
    // Create init_options
    CHECK(
        "Can't create support for node: " + nodeName,
        rclc_support_init(
            &support,
            0,
            NULL,
            &allocator));

    // Create node
    char* charNodeName = toCharArray(nodeName);

    CHECK(
        "Can't create micro-ros node: " + nodeName,
        rclc_node_init_default(
            &node,
            charNodeName,
            "",
            &support));
    
    delete charNodeName;

    CHECK(
        "Error to create executor for node: " + nodeName,
        rclc_executor_init(
            &executor,
            &support.context, 
            1, 
            &allocator
        )
    );

    logger.info("Connected to Ros2 Agent");
}


rclc_support_t* RosNodeManager::getSupport() {
    return &support;
};

rcl_node_t* RosNodeManager::getNode() {
    return &node;
};

rcl_allocator_t* RosNodeManager::getAllocator() {
    return &allocator;
}

rclc_executor_t* RosNodeManager::getExecutor() {
    return &executor;
}

void RosNodeManager::update(const uint64_t timeout_ns) {
    CHECK(
        "Cant't update executor",
        rclc_executor_spin_some(
            &executor, 
            timeout_ns
        )
    );
}

