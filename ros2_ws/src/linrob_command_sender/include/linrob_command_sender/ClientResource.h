#ifndef LINROB_CLIENTRESOURCE_H
#define LINROB_CLIENTRESOURCE_H

#include <hardware_interface/system_interface.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

#include <vector>

namespace linrob
{
class ClientResource : public hardware_interface::SystemInterface
{
public:
  /**
   * Constructor.
   */
  ClientResource();

  /**
   * Destructor.
   */
  ~ClientResource() override = default;

  /**
   * Initialization of the hardware interface. Reads robot description and assigns settings.
   * @param info robot settings specified in the robot description.
   * @return CallbackReturn that describes if process was successful.
   */
  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;

  /**
   * Configuration of the hardware interface.
   * @param previous_state previous state of the hardware interface.
   * @return CallbackReturn that describes if process was successful.
   */
  hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Activation of the hardware interface.
   * @param previous_state previous state of the hardware interface.
   * @return CallbackReturn that describes if process was successful.
   */
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Deactivation of the hardware interface.
   * @param previous_state previous state of the hardware interface.
   * @return CallbackReturn that describes if process was successful.
   */
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Checks if new command has been received and publishes if new command was received.
   * @param time current time.
   * @param period time since last call.
   * @return return_type that describes if process was successful.
   */
  hardware_interface::return_type write(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  /**
   * Reads the current state of the hardware interface.
   * @param time current time.
   * @param period time since last call.
   * @return return_type that describes if process was successful.
   */
  hardware_interface::return_type read(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  /**
   * Exports all state interfaces for this hardware interface.
   * @return vector of state interfaces.
   */
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  /**
   * Exports all command interfaces for this hardware interface.
   * @return vector of command interfaces.
   */
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

private:
  /**
   * Establishes ros communications for this hardware interface.
   * @param parameters parameters from robot description.
   */
  void createCommunications(const std::unordered_map<std::string, std::string>& parameters);

  /**
   * Checks if a new command has been received.
   * @return true if a new command has been received, false otherwise.
   */
  bool newCommandReceived();

  /**
   * Publishes the command to the robot command topic.
   */
  void sendCommand();

  /// Latest position command received.
  double_t mPositionCommand {0.0};

  /// Latest new position command received.
  double_t mLastPositionCommand {0.0};

  /// Node for the hardware interface. Required to establish ROS communications (publishers, subscribers).
  rclcpp::Node::SharedPtr mNode;

  /// Command publisher.
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr mCommandPublisher;
};
}

#endif // LINROB_CLIENTRESOURCE_H
