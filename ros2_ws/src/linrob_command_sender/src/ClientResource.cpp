#include "linrob_command_sender/ClientResource.h"

namespace linrob
{

const auto LINROB_CLIENT = "linrob_client";

ClientResource::ClientResource()
    : mNode(rclcpp::Node::make_shared("linrob_client_node"))
{}

hardware_interface::CallbackReturn ClientResource::on_init(const hardware_interface::HardwareInfo& info)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Initialize Resource STARTED.");
  const auto& params = info.hardware_parameters;

  createCommunications(params);

  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Initialize Resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn ClientResource::on_configure(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Configure Resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn ClientResource::on_activate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Start Resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn ClientResource::on_deactivate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Stop Resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type ClientResource::write(const rclcpp::Time&, const rclcpp::Duration&)
{
  // Check if a new command has been received.
  // If not, do not publish anything.
  if (!newCommandReceived())
  {
    return hardware_interface::return_type::OK;
  }

  sendCommand();

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type ClientResource::read(const rclcpp::Time&, const rclcpp::Duration&)
{
  return hardware_interface::return_type::OK;
}

std::vector<hardware_interface::StateInterface> ClientResource::export_state_interfaces()
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Export state interfaces STARTED.");
  std::vector<hardware_interface::StateInterface> state_interfaces;

  // Currently no state observation is needed. But robot description requires state interface so assigning nullptr to
  // state interface.
  state_interfaces.emplace_back("joint_1", "position", nullptr);
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Export state interfaces FINISHED.");
  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> ClientResource::export_command_interfaces()
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Export command interfaces STARTED.");

  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.emplace_back("joint_1", "position", &mPositionCommand);

  RCLCPP_INFO(rclcpp::get_logger(LINROB_CLIENT), "Export command interfaces FINISHED.");
  return command_interfaces;
}

void ClientResource::createCommunications(const std::unordered_map<std::string, std::string>& parameters)
{
  mCommandPublisher = mNode->create_publisher<std_msgs::msg::Float64MultiArray>(parameters.at("command_topic"), 10);
}

bool ClientResource::newCommandReceived()
{
  auto newCommandReceived = mPositionCommand != mLastPositionCommand;
  if (newCommandReceived)
  {
    mLastPositionCommand = mPositionCommand;
  }
  return newCommandReceived;
}

void ClientResource::sendCommand()
{
  std_msgs::msg::Float64MultiArray msg;
  msg.data.push_back(mPositionCommand);
  mCommandPublisher->publish(msg);
}
}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(linrob::ClientResource, hardware_interface::SystemInterface)
