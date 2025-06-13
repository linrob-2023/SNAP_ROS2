#include "linrob_axis/Resource.h"

#include "linrob_axis/Mode.h"

#include <linrob_axis/ctrlx_datalayer_helper.h>
#include <rclcpp/rclcpp.hpp>

#include <cstdint>
#include <iostream>

namespace linrob
{

const auto LINROB = "linrob";
constexpr std::chrono::milliseconds kStatePollInterval{100};
constexpr std::chrono::seconds kStateWaitTimeout{10};
constexpr std::chrono::seconds kSetModeSleep{2};

hardware_interface::CallbackReturn Resource::on_init(const hardware_interface::HardwareInfo& info)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Initialize resource STARTED...");

  // Connection.
  const auto& params = info.hardware_parameters;
  mConnection.ip = params.at("ip");
  mConnection.user = params.at("user");
  mConnection.password = params.at("password");
  mConnection.sslPort = std::stoi(params.at("ssl_port"));

  // Node addresses.
  registerDatalayerNode("new_position", params.at("new_position"));
  registerDatalayerNode("next_pos_index", params.at("next_pos_index"));
  registerDatalayerNode("execute_movements", params.at("execute_movements"));
  registerDatalayerNode("position", params.at("position"));
  registerDatalayerNode("velocity", params.at("velocity"));
  registerDatalayerNode("status", params.at("status"));
  registerDatalayerNode("read_mode", params.at("read_mode"));
  registerDatalayerNode("set_mode", params.at("set_mode"));

  // Hardware settings.
  mPositionSettings.initialIndex = static_cast<uint16_t>(std::stoi(params.at("initial_position_index")));
  mPositionSettings.nextPositionIndex = mPositionSettings.initialIndex;
  mPositionSettings.maxPositionIndices = static_cast<uint16_t>(std::stoi(params.at("max_position_indices")));
  mPositionSettings.newPositionsReceivedCount = 0U;
  mPositionSettings.executeMovementsOnIndex =
    static_cast<uint16_t>(std::stoi(params.at("execute_movements_on_next_index")));

  mExpectedDelayBetweenCommandsMs = static_cast<uint32_t>(1000.F / std::stof(params.at("update_frequency_hz")));

  // Logger.
  setLogLevel(params.at("log_level"));

  // Reset PLC buffer and index on init
  resetPlcBufferAndIndex();

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Initialize resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_configure(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Configure resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_activate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Activate resource STARTED...");
  auto connectionResult = connect();
  if (connectionResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    return connectionResult;
  }

  waitUntilRequiredNodesAreValid();

  auto axisStateResult = waitForAxisState(AxisState::STANDSTILL, kStateWaitTimeout);
  if (axisStateResult != hardware_interface::CallbackReturn::SUCCESS) {
      return axisStateResult;
  }

  auto setSystemModeResult = writeToDatalayerNode("set_mode", static_cast<uint8_t>(Mode::AUTO_EXTERNAL));
  if (!setSystemModeResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting %lld seconds for PLC to change operation mode...", kSetModeSleep.count());
  std::this_thread::sleep_for(kSetModeSleep);

  auto systemModeResult = waitForSystemMode("AUTO_EXTERNAL", kStateWaitTimeout);
  if (systemModeResult != hardware_interface::CallbackReturn::SUCCESS) {
      return systemModeResult;
  }

  // Reset PLC buffer and index
  resetPlcBufferAndIndex();

  auto nextPosIndexWriteResult = writeToDatalayerNode("next_pos_index", mPositionSettings.nextPositionIndex);
  if (!nextPosIndexWriteResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Resource activation FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_deactivate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Deactivate resource STARTED...");
  auto axisStateResult = checkAxisState(AxisState::STANDSTILL);

  if (axisStateResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    return axisStateResult;
  }

  auto setSystemModeResult = writeToDatalayerNode("set_mode", static_cast<uint8_t>(Mode::MANUAL));
  if (!setSystemModeResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting %lld seconds for PLC to change operation mode...", kSetModeSleep.count());
  std::this_thread::sleep_for(kSetModeSleep);

  auto systemModeResult = checkSystemMode("MANUAL");
  if (systemModeResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    return systemModeResult;
  }

  mPositionSettings.newPositionsReceivedCount = 0U;
  auto writeResult = writeToDatalayerNode("execute_movements", false);
  if (!writeResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Deactivate resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::return_type Resource::write(const rclcpp::Time& time, const rclcpp::Duration&)
{
  // Check if clock types matches.
  if (time.get_clock_type() != mLastPositionCommandTime.get_clock_type())
  {
    mLastPositionCommandTime = time;
  }

  // Check if new command is received.
  auto newPositionReceived = checkNewPositionReceived(time);
  if (!newPositionReceived)
  {
    return hardware_interface::return_type::OK;
  }

  // Increment next position index
  if (mPositionSettings.nextPositionIndex == kMaxPositionsExt)
    mPositionSettings.nextPositionIndex = 1;
  else
    ++mPositionSettings.nextPositionIndex;

  // PLC uses 1-based indices, C++ uses 0-based
  size_t pos = mPositionSettings.nextPositionIndex - 1;
  if (pos >= kMaxPositionsExt) pos = 0;

  // Update position command buffer with the new position
  mAxisTargetPositionsExt[pos] = mPositionCommand;

  // Send updated buffer and index to the PLC
  if (!writeToDatalayerNode("new_position", mAxisTargetPositionsExt))
    return hardware_interface::return_type::ERROR;
  if (!writeToDatalayerNode("next_pos_index", mPositionSettings.nextPositionIndex))
    return hardware_interface::return_type::ERROR;

  // Start movement if at least 2 new positions have been received
  if (mPositionSettings.newPositionsReceivedCount >= 2)
  {
    if (!writeToDatalayerNode("execute_movements", true))
      return hardware_interface::return_type::ERROR;
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type Resource::read(const rclcpp::Time&, const rclcpp::Duration&)
{
  auto updateResult = true;
  // Update current system and axis info.
  updateResult &= updateDataFromNode("status", comm::datalayer::VariantType::ARRAY_OF_INT32);
  updateResult &= updateDataFromNode("read_mode", comm::datalayer::VariantType::STRING);

  // Update current position and velocity info.
  updateResult &= updateDataFromNode("position", comm::datalayer::VariantType::ARRAY_OF_FLOAT64);
  updateResult &= updateDataFromNode("velocity", comm::datalayer::VariantType::ARRAY_OF_FLOAT64);
  if (!updateResult)
  {
    return hardware_interface::return_type::ERROR;
  }

  updateState();

  return hardware_interface::return_type::OK;
}

std::vector<hardware_interface::StateInterface> Resource::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> state_interfaces;
  state_interfaces.emplace_back("joint_1", "position", &mState.at("position"));
  state_interfaces.emplace_back("joint_1", "velocity", &mState.at("velocity"));
  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> Resource::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.emplace_back("joint_1", "position", &mPositionCommand);
  return command_interfaces;
}

hardware_interface::CallbackReturn Resource::waitForAxisState(AxisState expectedState, std::chrono::milliseconds timeout)
{
    auto start = std::chrono::steady_clock::now();
    const std::chrono::milliseconds pollInterval(kStatePollInterval);

    while (std::chrono::steady_clock::now() - start < timeout) {
        auto result = checkAxisState(expectedState);
        if (result == hardware_interface::CallbackReturn::SUCCESS) {
            RCLCPP_INFO(rclcpp::get_logger(LINROB), "Axis reached expected state: %u", static_cast<unsigned int>(expectedState));
            return result;
        }
        auto& statusData = mConnection.datalayerNodeMap.at("status");
        auto axisStatus = static_cast<AxisState>(variantDataToVector<int>(statusData.second)[0U]);
        if (axisStatus == AxisState::ERROR_STOP) {
            RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Axis is in ERROR_STOP state, aborting wait.");
            return hardware_interface::CallbackReturn::ERROR;
        }
        RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting for axis state %u, current: %u", static_cast<unsigned int>(expectedState), static_cast<unsigned int>(axisStatus));
        std::this_thread::sleep_for(pollInterval);
    }
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Timeout waiting for axis state %u", static_cast<unsigned int>(expectedState));
    return hardware_interface::CallbackReturn::FAILURE;
}

hardware_interface::CallbackReturn Resource::waitForSystemMode(const std::string& expectedMode, std::chrono::milliseconds timeout)
{
    auto start = std::chrono::steady_clock::now();
    const std::chrono::milliseconds pollInterval(kStatePollInterval);

    while (std::chrono::steady_clock::now() - start < timeout) {
        auto result = checkSystemMode(expectedMode);
        if (result == hardware_interface::CallbackReturn::SUCCESS) {
            RCLCPP_INFO(rclcpp::get_logger(LINROB), "System reached expected mode: %s", expectedMode.c_str());
            return result;
        }
        RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting for system mode '%s'", expectedMode.c_str());
        std::this_thread::sleep_for(pollInterval);
    }
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Timeout waiting for system mode '%s'", expectedMode.c_str());
    return hardware_interface::CallbackReturn::FAILURE;
}

hardware_interface::CallbackReturn Resource::connect()
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Connecting to datalayer...");
  auto connectionString =
    getConnectionString(mConnection.ip, mConnection.user, mConnection.password, mConnection.sslPort);

  mDatalayerSystem.start(false);
  auto client = mDatalayerSystem.factory()->createClient3(connectionString);
  mClient = std::unique_ptr<comm::datalayer::IClient>(client);
  if (mClient == nullptr)
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to create client");
    return hardware_interface::CallbackReturn::ERROR;
  }
  if (!mClient->isConnected())
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to connect to client");
    return hardware_interface::CallbackReturn::FAILURE;
  }
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Connected to the datalayer.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::checkAxisState(AxisState expectedState)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Checking axis state...");
  auto statusUpdateResult = updateDataFromNode("status", comm::datalayer::VariantType::ARRAY_OF_INT32);
  if (!statusUpdateResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  // Check updated data.
  auto& statusData = mConnection.datalayerNodeMap.at("status");

  auto axisStatus = static_cast<AxisState>(variantDataToVector<int>(statusData.second)[0U]);
  if (axisStatus != expectedState)
  {
    RCLCPP_INFO(rclcpp::get_logger(LINROB),
                "Axis is not in expected state. Expected: %u, Actual: %u",
                static_cast<unsigned int>(expectedState),
                static_cast<unsigned int>(axisStatus));
    return hardware_interface::CallbackReturn::FAILURE;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Axis state: %u", static_cast<unsigned int>(axisStatus));
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::checkSystemMode(const std::string& expectedMode)
{
  auto modeUpdateResult = updateDataFromNode("read_mode", comm::datalayer::VariantType::STRING);
  if (!modeUpdateResult)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  // Check updated data.
  const auto& modeData = mConnection.datalayerNodeMap.at("read_mode");
  auto mode = std::string(modeData.second);
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "System mode: %s", mode.c_str());
  if (mode != expectedMode)
  {
    RCLCPP_INFO(rclcpp::get_logger(LINROB), ("System is not in expected mode: " + expectedMode).c_str());
    return hardware_interface::CallbackReturn::FAILURE;
  }
  return hardware_interface::CallbackReturn::SUCCESS;
}

void Resource::registerDatalayerNode(const std::string& key, const std::string& address)
{
  mConnection.datalayerNodeMap.emplace(key, std::make_pair(address, comm::datalayer::Variant()));
}

bool Resource::updateDataFromNode(const std::string& key, comm::datalayer::VariantType expectedType)
{
  auto& data = mConnection.datalayerNodeMap.at(key);
  RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Reading data from %s...", data.first.c_str());
  auto result = mClient->readSync(data.first, &data.second);
  RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Data received from %s.", data.first.c_str());
  RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Result is DL_OK %s.", std::to_string(result == DL_OK).c_str());
  RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
               "Result type is %u. Expected type is %u.",
               static_cast<unsigned int>(data.second.getType()),
               static_cast<unsigned int>(expectedType));
  if (result != DL_OK)
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to read data at %s. %s", data.first.c_str(), result.toString());
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Failed to update. Returning FALSE.");
    return false;
  }
  if (data.second.getType() != expectedType)
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB),
                 "Invalid type for data at %s. Expected: %u; Actual: %u",
                 data.first.c_str(),
                 static_cast<unsigned int>(expectedType),
                 static_cast<unsigned int>(data.second.getType()));
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Failed to update. Returning FALSE.");
    return false;
  }
  RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Update successful. Returning TRUE.");
  return true;
}

void Resource::resetPlcBufferAndIndex()
{
  if (!updateDataFromNode("position", comm::datalayer::VariantType::ARRAY_OF_FLOAT64)) {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to fetch axis position before resetting PLC buffer!");
  }
  updateState();
  resetAxisTargetPositionsExt();
  if (mPositionSettings.initialIndex == 1)
    mPositionSettings.nextPositionIndex = kMaxPositionsExt;
  else
    mPositionSettings.nextPositionIndex = mPositionSettings.initialIndex - 1;
  writeToDatalayerNode("new_position", mAxisTargetPositionsExt);
  writeToDatalayerNode("next_pos_index", mPositionSettings.nextPositionIndex);
}

void Resource::updateState()
{
  mState.at("position") =
    static_cast<double>(variantDataToVector<float>(mConnection.datalayerNodeMap.at("position").second)[0U]);
  mState.at("velocity") =
    static_cast<double>(variantDataToVector<float>(mConnection.datalayerNodeMap.at("velocity").second)[0U]);
}

bool Resource::checkNewPositionReceived(const rclcpp::Time& currentTime)
{
  // Check if last position and current positions are different.
  auto positionsReceived = true;
  positionsReceived &= mPositionCommand != mLastPositionCommand;

  // Position not received.
  // Check if we should still wait for new position or no new positions will be received.
  auto newCommandsExpected = true;
  if (!positionsReceived)
  {
    // Assuming we always receive new positions at specified frequency. Therefore each new position is expected to be
    // received within the specified time (1 sec / frequency). Requires reliable connection between this interface and
    // hardware.
    auto expectedDurationBetweenCommands = rclcpp::Duration(std::chrono::milliseconds(mExpectedDelayBetweenCommandsMs));
    newCommandsExpected = currentTime - mLastPositionCommandTime < expectedDurationBetweenCommands;
  }
  else
  {
    mLastPositionCommand = mPositionCommand;
    mLastPositionCommandTime = currentTime;
    ++mPositionSettings.newPositionsReceivedCount;
  }

  // Check if at least 3 new positions are received.
  if (newCommandsExpected && (mPositionSettings.newPositionsReceivedCount == mPositionSettings.executeMovementsOnIndex))
  {
    mMovementExecutionStopped = false;
    auto execMovementsResult = writeToDatalayerNode("execute_movements", true);
    if (!execMovementsResult)
    {
      return false;
    }
  }

  // Stopped receiving new positions.
  if (!newCommandsExpected)
  {
    mPositionSettings.newPositionsReceivedCount = 0U;
    if (mMovementExecutionStopped)
    {
      return false;
    }
    auto newPositionWriteResult = writeToDatalayerNode("new_position", mState.at("position"));
    if (!newPositionWriteResult)
    {
      return false;
    }
    auto execMovementsResult = writeToDatalayerNode("execute_movements", false);
    if (!execMovementsResult)
    {
      return false;
    }
    mMovementExecutionStopped = true;
  }

  return true;
}

void Resource::waitUntilRequiredNodesAreValid()
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting until required nodes are valid...");
  auto wait = true;
  while (wait)
  {
    // Slow down the loop to avoid busy waiting.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Check if required addresses are valid.
    auto updateResult = true;
    updateResult &= updateDataFromNode("status", comm::datalayer::VariantType::ARRAY_OF_INT32);
    updateResult &= updateDataFromNode("read_mode", comm::datalayer::VariantType::STRING);
    updateResult &= updateDataFromNode("position", comm::datalayer::VariantType::ARRAY_OF_FLOAT64);
    updateResult &= updateDataFromNode("velocity", comm::datalayer::VariantType::ARRAY_OF_FLOAT64);
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
                 "Update final result value: %s; If all valid: 1",
                 std::to_string(updateResult).c_str());

    wait = !updateResult;
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Wait value: %s", std::to_string(wait).c_str());
  }
}

void Resource::setLogLevel(const std::string& level)
{
  auto logger = rclcpp::get_logger(LINROB);
  auto loggerLevel = rclcpp::Logger::Level::Info;
  if (level == "DEBUG")
  {
    loggerLevel = rclcpp::Logger::Level::Debug;
  }
  else if (level == "INFO")
  {
    loggerLevel = rclcpp::Logger::Level::Info;
  }
  else if (level == "WARN")
  {
    loggerLevel = rclcpp::Logger::Level::Warn;
  }
  else if (level == "ERROR")
  {
    loggerLevel = rclcpp::Logger::Level::Error;
  }
  else if (level == "FATAL")
  {
    loggerLevel = rclcpp::Logger::Level::Fatal;
  }
  else
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Invalid log level: %s", level.c_str());
    return;
  }
  logger.set_level(loggerLevel);
}
}

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(linrob::Resource, hardware_interface::SystemInterface)
