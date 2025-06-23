#ifndef LINROB_RESOURCE_H
#define LINROB_RESOURCE_H

#include "linrob_axis/AxisStates.h"
#include "linrob_axis/Connection.h"
#include "linrob_axis/PositionSettings.h"
#include "linrob_axis/VariantType.h"

#include <comm/datalayer/datalayer_system.h>
#include <hardware_interface/system_interface.hpp>
#include <rclcpp/duration.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/time.hpp>
#include <unordered_map>
#include <cmath>

namespace linrob
{
/**
 * Data layer class for Linrob axis hardware.
 */
class Resource : public hardware_interface::SystemInterface
{
public:
  /**
   * Constructor.
   */
  Resource() = default;

  /**
   * Destructor.
   */
  ~Resource() override = default;

  /**
   * Initialization of the hardware interface from data parsed from the robot's URDF.
   * @param info structure with data from URDF.
   * @return State of the function.
   */
  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;

  /**
   * Callback function for configure transition.
   * @param previous_state previous state of the hardware interface.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Function for activating the hardware.AxisState
   * @param previous_state previous state of the hardware interface.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Function for turning off the hardware.
   * @param previous_state previous state of the hardware interface.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

  /**
   * Function for writing latest command to the hardware.
   * @param time time at the start of this control loop iteration.
   * @param period measured time taken by the last control loop iteration.
   * @return state of the function.
   */
  hardware_interface::return_type write(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  /**
   * Function for reading the current state of the hardware.
   * @param time time at the start of this control loop iteration.
   * @param period measured time taken by the last control loop iteration.
   * @return state of the function.
   */
  hardware_interface::return_type read(const rclcpp::Time& time, const rclcpp::Duration& period) override;

  /**
   * Disconnects the datalayer client
   * @return state of the function.
   */
  void disconnect();

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

  /**
   * Waits for the axis state to reach the expected state within a timeout.
   * Returns SUCCESS if reached, FAILURE on timeout, ERROR if error state detected.
   */
  hardware_interface::CallbackReturn waitForAxisState(AxisState expectedState, std::chrono::milliseconds timeout);

  /**
   * Waits for the system mode to reach the expected mode within a timeout.
   * Returns SUCCESS if reached, FAILURE on timeout.
   */
  hardware_interface::CallbackReturn waitForSystemMode(const std::string& expectedMode, std::chrono::milliseconds timeout);

private:
  /**
   * Creates data pair with datalayer node address and variant type for storing data read from datalayer node and stores
   * created pair in the map at specified key.
   * @param key key for the datalayer node.
   * @param address address of the datalayer node.
   */
  void registerDatalayerNode(const std::string& key, const std::string& address);

  /**
   * Reads datalayer node.
   * @param key key for the datalayer node.
   * @param expectedType expected variant type of the data.
   * @return true if the data was read successfully, false otherwise.
   */
  bool updateDataFromNode(const std::string& key, comm::datalayer::VariantType expectedType);

  /**
   * Establishes connection to the datalayer node.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn connect();

  /**
   * Checks if the axis is in the expected state.
   * @param expectedState expected axis state.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn checkAxisState(AxisState expectedState);

  /**
   * Checks if the system is in the expected mode.
   * @param expectedMode expected system mode.
   * @return state of the function.
   */
  hardware_interface::CallbackReturn checkSystemMode(const std::string& expectedMode);

  /**
   * Updates the state of the axis. Converts data stored in the connection map to the state interface.
   * Stored data: current position and velocity.
   * @return state of the function.
   */
  void updateState();

  /**
   * Checks if new position was received.
   * @param currentTime current time.
   * @return true if new position was received, false otherwise.
   */
  bool checkNewPositionReceived(const rclcpp::Time& currentTime);

  /**
   * Writes data to the specified datalayer node.
   * @tparam T type of the data to be written.
   * @param key key for the datalayer node.
   * @param value value to be written to the datalayer node.
   * @return true if the data was written successfully, false otherwise.
   */
  template <typename T>
  bool writeToDatalayerNode(const std::string& key, const T& value);

  /**
   * Converts data soted in the datalayer Variant object to vector of specified type.
   * @tparam T type of the data to be converted.
   * @param data datalayer Variant object.
   * @return vector of converted data.
   */
  template <typename T>
  std::vector<T> variantDataToVector(const comm::datalayer::Variant& data);

  /**
   * Waits until state, status and mode datalayer nodes are valid.
   */
  void waitUntilRequiredNodesAreValid();

  /**
   * Sets the log level for the datalayer system.
   *
   * Possible log levels:
   * - DEBUG
   * - INFO
   * - WARN
   * - ERROR
   * - FATAL
   *
   * @param level log level to be set.
   *
   */
  void setLogLevel(const std::string& level);

  /**
   * Helper to reset the axis target positions buffer (local only).
   * Fills the buffer with the current position (rounded to 4 decimal places).
   */
  inline void resetAxisTargetPositionsExt() {
    double pos = std::round(mState.at("position") * 10000.0) / 10000.0;
    for (size_t i = 0; i < kMaxPositionsExt; ++i) {
      mAxisTargetPositionsExt[i] = pos;
    }
  }

  /**
   * Helper to reset PLC buffer and index (writes to PLC).
   * Fills local buffer with current position, resets index, and writes both to the PLC.
   */
  bool resetPlcBufferAndIndex();

  /// Connection settings.
  linrob::Connection mConnection;

  /// Datalayer system.
  comm::datalayer::DatalayerSystem mDatalayerSystem;

  /// Datalayer client.
  std::unique_ptr<comm::datalayer::IClient> mClient = nullptr;

  /// Hardware state interface.
  std::unordered_map<std::string, double> mState {{"position", 0.0}, {"velocity", 0.0}};

  // Hardware command interface.
  double mPositionCommand {0.0};

  /// Last new position received.
  double mLastPositionCommand {0.0};

  /// Last time when new position command was received.
  rclcpp::Time mLastPositionCommandTime;

  /// Position command settings for observing and managing system state.
  PositionSettings mPositionSettings;

  /// Expected max delay in milliseconds between commands.
  uint32_t mExpectedDelayBetweenCommandsMs {0U};

  /// Flag to mark if the movement execution was already stopped.
  bool mMovementExecutionStopped {true};

  /// Buffer for array write to new_position (ARRAY[LREAL] in PLC).
  static constexpr size_t kMaxPositionsExt = 1000;
  double mAxisTargetPositionsExt[kMaxPositionsExt] = {0.0};
};

template <typename T>
std::vector<T> Resource::variantDataToVector(const comm::datalayer::Variant& data)
{
  auto expectedType = VARIANT_ARRAY_TYPE_MAP.at(typeid(T).name());
  if (data.getType() != expectedType)
  {
    RCLCPP_ERROR(rclcpp::get_logger("linrob"),
                 "Invalid type for data. Expected: %u; Actual: %u",
                 static_cast<unsigned int>(expectedType),
                 static_cast<unsigned int>(data.getType()));
    return {};
  }
  auto size = data.getCount();
  auto typeData = static_cast<const T*>(static_cast<const void*>(data.getData()));
  std::vector<T> result(typeData, typeData + size);
  return result;
}

template <typename T>
bool Resource::writeToDatalayerNode(const std::string& key, const T& value)
{
  auto& data = mConnection.datalayerNodeMap.at(key);
  data.second.setValue(value);
  auto writeResult = mClient->writeSync(data.first, &data.second);
  if (writeResult != DL_OK)
  {
    RCLCPP_ERROR(
      rclcpp::get_logger("linrob"), "Failed to write data at %s. %s", data.first.c_str(), writeResult.toString());
    return false;
  }
  return true;
}
}

#endif // LINROB_RESOURCE_H
