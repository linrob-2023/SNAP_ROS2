#include "linrob_axis/axis_controller.hpp"
#include <hardware_interface/loaned_command_interface.hpp>
#include <hardware_interface/loaned_state_interface.hpp>

namespace linrob_controllers
{

AxisController::AxisController()
: controller_interface::ControllerInterface(),
  joint_name_("joint_1")
{
}

controller_interface::CallbackReturn AxisController::on_init()
{
  try {
    // Declare parameters
    auto_declare<std::string>("joint_name", joint_name_);
  } catch (const std::exception & e) {
    fprintf(stderr, "Exception thrown during init stage with message: %s \n", e.what());
    return controller_interface::CallbackReturn::ERROR;
  }

  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn AxisController::on_configure(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  // Get parameters
  joint_name_ = get_node()->get_parameter("joint_name").as_string();

  // Build interface names
  virtual_reset_interface_name_ = joint_name_ + "/virtual_reset";
  virtual_reference_interface_name_ = joint_name_ + "/virtual_reference";
  virtual_stop_interface_name_ = joint_name_ + "/virtual_stop";
  error_code_interface_name_ = joint_name_ + "/error_code";

  // Create service servers
  reset_service_ = get_node()->create_service<linrob_axis::srv::ResetAxis>(
    "~/reset_axis",
    std::bind(&AxisController::resetAxisService, this,
              std::placeholders::_1, std::placeholders::_2));

  reference_service_ = get_node()->create_service<linrob_axis::srv::ReferenceAxis>(
    "~/reference_axis",
    std::bind(&AxisController::referenceAxisService, this,
              std::placeholders::_1, std::placeholders::_2));

  stop_service_ = get_node()->create_service<linrob_axis::srv::StopAxis>(
    "~/stop_axis",
    std::bind(&AxisController::stopAxisService, this,
              std::placeholders::_1, std::placeholders::_2));

  // Create error code publisher
  error_code_publisher_ = get_node()->create_publisher<std_msgs::msg::UInt32>(
    "~/error_code", 10);

  RCLCPP_INFO(get_node()->get_logger(), "Axis control controller configured");
  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn AxisController::on_activate(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  // Clear any pending commands
  pending_reset_.store(false);
  pending_reference_.store(false);
  pending_stop_.store(false);

  RCLCPP_INFO(get_node()->get_logger(), "Axis control controller activated");
  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn AxisController::on_deactivate(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(get_node()->get_logger(), "Axis control controller deactivated");
  return controller_interface::CallbackReturn::SUCCESS;
}

controller_interface::InterfaceConfiguration
AxisController::command_interface_configuration() const
{
  controller_interface::InterfaceConfiguration config;
  config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

  config.names.push_back(virtual_reset_interface_name_);
  config.names.push_back(virtual_reference_interface_name_);
  config.names.push_back(virtual_stop_interface_name_);

  return config;
}

controller_interface::InterfaceConfiguration
AxisController::state_interface_configuration() const
{
  controller_interface::InterfaceConfiguration config;
  config.type = controller_interface::interface_configuration_type::INDIVIDUAL;

  config.names.push_back(error_code_interface_name_);

  return config;
}

controller_interface::return_type AxisController::update(
  const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
{
  if (pending_reset_.exchange(false)) {
    command_interfaces_[0].set_value(1.0);  // virtual_reset
    RCLCPP_INFO(get_node()->get_logger(), "Reset command sent to hardware");
  }

  if (pending_reference_.exchange(false)) {
    command_interfaces_[1].set_value(1.0);  // virtual_reference
    RCLCPP_INFO(get_node()->get_logger(), "Reference command sent to hardware");
  }

  if (pending_stop_.exchange(false)) {
    command_interfaces_[2].set_value(1.0);  // virtual_stop
    RCLCPP_INFO(get_node()->get_logger(), "Stop command sent to hardware");
  }

  // Read and publish error code
  uint32_t current_error_code = static_cast<uint32_t>(state_interfaces_[0].get_value());
  if (current_error_code != last_error_code_) {
    auto msg = std_msgs::msg::UInt32();
    msg.data = current_error_code;
    error_code_publisher_->publish(msg);
    last_error_code_ = current_error_code;

    if (current_error_code != 0) {
      RCLCPP_WARN(get_node()->get_logger(), "Error code: 0x%04X", current_error_code);
    }
  }

  return controller_interface::return_type::OK;
}

void AxisController::resetAxisService(
  const std::shared_ptr<linrob_axis::srv::ResetAxis::Request> /*request*/,
  std::shared_ptr<linrob_axis::srv::ResetAxis::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Reset axis service called");

  pending_reset_.store(true);

  response->success = true;
  response->message = "Reset command queued for execution";
}

void AxisController::referenceAxisService(
  const std::shared_ptr<linrob_axis::srv::ReferenceAxis::Request> /*request*/,
  std::shared_ptr<linrob_axis::srv::ReferenceAxis::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Reference axis service called");

  pending_reference_.store(true);

  response->success = true;
  response->message = "Reference command queued for execution";
}

void AxisController::stopAxisService(
  const std::shared_ptr<linrob_axis::srv::StopAxis::Request> /*request*/,
  std::shared_ptr<linrob_axis::srv::StopAxis::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Stop axis service called");

  pending_stop_.store(true);

  response->success = true;
  response->message = "Stop command queued for execution";
}

}  // namespace linrob_controllers

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
  linrob_controllers::AxisController, controller_interface::ControllerInterface)
