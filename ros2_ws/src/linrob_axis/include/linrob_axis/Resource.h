#ifndef LINROB_RESOURCE_H
#define LINROB_RESOURCE_H

#include "linrob_axis/Connection.h"

#include <comm/datalayer/datalayer_system.h>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp/duration.hpp>
#include <rclcpp/time.hpp>

namespace linrob
{
class Resource : public hardware_interface::SystemInterface
{
public:
  Resource() = default;
  ~Resource() override = default;

  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;
  hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

  hardware_interface::return_type write(const rclcpp::Time& time, const rclcpp::Duration& period) override;
  hardware_interface::return_type read(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

private:
  hardware_interface::CallbackReturn connect();

private:
  linrob::Connection mConnection;

  // TODO: Receiving:
  // symbol lookup error: /home/matomai/Work/Linrob/ros2_ws/install/linrob_axis/lib/liblinrob_axis.so: undefined
  // symbol: _ZN4comm9datalayer15DatalayerSystemC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
  comm::datalayer::DatalayerSystem mDatalayerSystem;
  std::shared_ptr<comm::datalayer::IClient> mClient = nullptr;

  double mPosition {0.0};
  double mVelocity {0.0};
  double mEffort {0.0};
};
}

#endif // LINROB_RESOURCE_H
