#ifndef LINROB_AXIS_CONTROLLER_H
#define LINROB_AXIS_CONTROLLER_H

#include <controller_interface/controller_interface.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/u_int32.hpp>

#include "linrob_axis/srv/reset_axis.hpp"
#include "linrob_axis/srv/reference_axis.hpp"
#include "linrob_axis/srv/stop_axis.hpp"

#include <atomic>
#include <memory>

namespace linrob_controllers
{

class AxisController : public controller_interface::ControllerInterface
{
public:
  AxisController();

  controller_interface::InterfaceConfiguration command_interface_configuration() const override;
  controller_interface::InterfaceConfiguration state_interface_configuration() const override;

  controller_interface::return_type update(
    const rclcpp::Time& time, const rclcpp::Duration& period) override;

  controller_interface::CallbackReturn on_init() override;
  controller_interface::CallbackReturn on_configure(
    const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State& previous_state) override;
  controller_interface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State& previous_state) override;

private:
  // Service callback functions
  void resetAxisService(
    const std::shared_ptr<linrob_axis::srv::ResetAxis::Request> request,
    std::shared_ptr<linrob_axis::srv::ResetAxis::Response> response);

  void referenceAxisService(
    const std::shared_ptr<linrob_axis::srv::ReferenceAxis::Request> request,
    std::shared_ptr<linrob_axis::srv::ReferenceAxis::Response> response);

  void stopAxisService(
    const std::shared_ptr<linrob_axis::srv::StopAxis::Request> request,
    std::shared_ptr<linrob_axis::srv::StopAxis::Response> response);

  // Service servers
  rclcpp::Service<linrob_axis::srv::ResetAxis>::SharedPtr reset_service_;
  rclcpp::Service<linrob_axis::srv::ReferenceAxis>::SharedPtr reference_service_;
  rclcpp::Service<linrob_axis::srv::StopAxis>::SharedPtr stop_service_;

  // Publishers for error code monitoring
  rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr error_code_publisher_;

  // Command interface names
  std::string virtual_reset_interface_name_;
  std::string virtual_reference_interface_name_;
  std::string virtual_stop_interface_name_;

  // State interface names
  std::string error_code_interface_name_;

  // Command buffers (thread-safe)
  std::atomic<bool> pending_reset_{false};
  std::atomic<bool> pending_reference_{false};
  std::atomic<bool> pending_stop_{false};

  // Last error code for publishing
  uint32_t last_error_code_{0};

  // Joint name from parameters
  std::string joint_name_;
};

}  // namespace linrob_controllers

#endif  // LINROB_AXIS_CONTROLLER_H
