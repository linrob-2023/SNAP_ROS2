#include "linrob_axis/axis_controller.hpp"
#include <hardware_interface/loaned_command_interface.hpp>
#include <hardware_interface/loaned_state_interface.hpp>



namespace {
constexpr std::chrono::milliseconds kServiceErrorWait{500};

bool isServiceError(uint32_t error_code) {
  char hex_code[16];
  snprintf(hex_code, sizeof(hex_code), "%08X", error_code);
  return error_code != 0 && (strncmp(hex_code, "080F", 4) == 0 || strncmp(hex_code, "090F", 4) == 0);
}

std::string errorMessage(const char* action, uint32_t error_code) {
  char hex_code[16];
  snprintf(hex_code, sizeof(hex_code), "%08X", error_code);
  return std::string(action) + " command failed with error code: 0x" + hex_code;
}
}

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
  virtual_start_motion_interface_name_ = joint_name_ + "/virtual_start_motion";
  virtual_target_position_interface_name_ = joint_name_ + "/virtual_target_position";
  virtual_target_velocity_interface_name_ = joint_name_ + "/virtual_target_velocity";
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

  start_motion_service_ = get_node()->create_service<linrob_axis::srv::StartMotion>(
    "~/start_motion",
    std::bind(&AxisController::startMotionService, this,
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
  pending_start_motion_.store(false);
  pending_target_position_.store(0);
  pending_target_velocity_.store(0.0);

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
  config.names.push_back(virtual_start_motion_interface_name_);
  config.names.push_back(virtual_target_position_interface_name_);
  config.names.push_back(virtual_target_velocity_interface_name_);

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
  } else {
    command_interfaces_[0].set_value(0.0);
  }

  if (pending_reference_.exchange(false)) {
    command_interfaces_[1].set_value(1.0);  // virtual_reference
    RCLCPP_INFO(get_node()->get_logger(), "Reference command sent to hardware");
  } else {
    command_interfaces_[1].set_value(0.0);
  }

  if (pending_stop_.exchange(false)) {
    command_interfaces_[2].set_value(1.0);  // virtual_stop
    RCLCPP_INFO(get_node()->get_logger(), "Stop command sent to hardware");
  } else {
    command_interfaces_[2].set_value(0.0);
  }

  // Index 3: virtual_start_motion (boolean trigger)
  if (pending_start_motion_.exchange(false)) {
    command_interfaces_[3].set_value(1.0);
    RCLCPP_INFO(get_node()->get_logger(), "Start motion command sent to hardware");
  } else {
    command_interfaces_[3].set_value(0.0);
  }

  // Index 4: virtual_target_position (uint8 encoded as double)
  command_interfaces_[4].set_value(static_cast<double>(pending_target_position_.load()));

  // Index 5: virtual_target_velocity
  command_interfaces_[5].set_value(pending_target_velocity_.load());

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

  std::this_thread::sleep_for(kServiceErrorWait);
  uint32_t error_code = static_cast<uint32_t>(state_interfaces_[0].get_value());
  if (isServiceError(error_code)) {
    response->success = false;
    response->message = errorMessage("Reset", error_code);
  } else {
    response->success = true;
    response->message = "Reset command queued for execution";
  }
}

void AxisController::referenceAxisService(
  const std::shared_ptr<linrob_axis::srv::ReferenceAxis::Request> /*request*/,
  std::shared_ptr<linrob_axis::srv::ReferenceAxis::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Reference axis service called");

  pending_reference_.store(true);

  std::this_thread::sleep_for(kServiceErrorWait);
  uint32_t error_code = static_cast<uint32_t>(state_interfaces_[0].get_value());
  if (isServiceError(error_code)) {
    response->success = false;
    response->message = errorMessage("Reference", error_code);
  } else {
    response->success = true;
    response->message = "Reference command queued for execution";
  }
}

void AxisController::stopAxisService(
  const std::shared_ptr<linrob_axis::srv::StopAxis::Request> /*request*/,
  std::shared_ptr<linrob_axis::srv::StopAxis::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Stop axis service called");

  pending_stop_.store(true);

  std::this_thread::sleep_for(kServiceErrorWait);
  uint32_t error_code = static_cast<uint32_t>(state_interfaces_[0].get_value());
  if (isServiceError(error_code)) {
    response->success = false;
    response->message = errorMessage("Stop", error_code);
  } else {
    response->success = true;
    response->message = "Stop command queued for execution";
  }
}

void AxisController::startMotionService(
  const std::shared_ptr<linrob_axis::srv::StartMotion::Request> request,
  std::shared_ptr<linrob_axis::srv::StartMotion::Response> response)
{
  RCLCPP_INFO(get_node()->get_logger(), "Start motion service called (start=%s, target=%f, velocity=%f)",
              request->start ? "true" : "false", request->target_position, request->velocity);

  if (request->start) {
    pending_start_motion_.store(true);
  }
  pending_target_position_.store(request->target_position);
  pending_target_velocity_.store(request->velocity);

  std::this_thread::sleep_for(kServiceErrorWait);
  uint32_t error_code = static_cast<uint32_t>(state_interfaces_[0].get_value());
  if (isServiceError(error_code)) {
    response->success = false;
    response->message = errorMessage("Start motion", error_code);
  } else {
    response->success = true;
    response->message = "Start motion command queued for execution";
  }
}

}  // namespace linrob_controllers

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
  linrob_controllers::AxisController, controller_interface::ControllerInterface)
