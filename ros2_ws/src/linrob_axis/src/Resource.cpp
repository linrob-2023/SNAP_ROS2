#include "linrob_axis/Resource.h"

#include "linrob_axis/ctrlx_datalayer_helper.h"

#include <rclcpp/rclcpp.hpp>

#include <iostream>

namespace linrob
{
hardware_interface::CallbackReturn Resource::on_init(const hardware_interface::HardwareInfo&)
{
  // TODO: Read parameters from config file.
  // TODO: Initialize required resources.

  mConnection.ip = "127.0.0.1";
  mConnection.user = "boschrexroth";
  mConnection.password = "boschrexroth";
  mConnection.sslPort = 443;

  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Initializing resource");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_configure(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Configuring resource");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_activate(const rclcpp_lifecycle::State&)
{
  // Activate the datalayer system and create a client connection.
  auto connectionResult = connect();
  if (connectionResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    RCLCPP_ERROR(rclcpp::get_logger("linrob"), "Failed to connect to datalayer");
    return connectionResult;
  }
  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Activating resource");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_deactivate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Deactivating resource");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type Resource::write(const rclcpp::Time&, const rclcpp::Duration&)
{
  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Writing to resource");
  return hardware_interface::return_type::OK;
}

hardware_interface::return_type Resource::read(const rclcpp::Time&, const rclcpp::Duration&)
{
  RCLCPP_INFO(rclcpp::get_logger("linrob"), "Reading from resource");
  return hardware_interface::return_type::OK;
}

std::vector<hardware_interface::StateInterface> Resource::export_state_interfaces()
{
  // TODO: Temporary implementation. Requires to know how to bind this with actual hardware.
  std::vector<hardware_interface::StateInterface> state_interfaces;
  state_interfaces.emplace_back("joint_1", "position", &mPosition);
  state_interfaces.emplace_back("joint_1", "velocity", &mVelocity);
  state_interfaces.emplace_back("joint_1", "effort", &mEffort);
  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> Resource::export_command_interfaces()
{
  // TODO: Temporary implementation. Requires to know how to bind this with actual hardware.
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.emplace_back("joint_1", "position", &mPosition);
  command_interfaces.emplace_back("joint_1", "effort", &mEffort);
  return command_interfaces;
}

hardware_interface::CallbackReturn Resource::connect()
{
  auto connectionString =
    getConnectionString(mConnection.ip, mConnection.user, mConnection.password, mConnection.sslPort);

  mClient = std::unique_ptr<comm::datalayer::IClient>(mDatalayerSystem.factory()->createClient(connectionString));
  if (mClient == nullptr)
  {
    RCLCPP_ERROR(rclcpp::get_logger("linrob"), "Failed to create client");
    return hardware_interface::CallbackReturn::ERROR;
  }
  if (!mClient->isConnected())
  {
    RCLCPP_ERROR(rclcpp::get_logger("linrob"), "Failed to connect to client");
    return hardware_interface::CallbackReturn::FAILURE;
  }
  return hardware_interface::CallbackReturn::SUCCESS;
}
}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(linrob::Resource, hardware_interface::SystemInterface)
