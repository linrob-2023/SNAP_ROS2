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
constexpr std::chrono::seconds kAxisReadinessCheckInterval{2};

Resource::~Resource()
{
  disconnect();
}

hardware_interface::CallbackReturn Resource::on_init(const hardware_interface::HardwareInfo& info)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Initialize resource STARTED...");

  if (hardware_interface::SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS)
  {
    return hardware_interface::CallbackReturn::ERROR;
  }

  // Connection.
  const auto& params = info.hardware_parameters;
  mConnection.ip = params.at("ip");
  mConnection.user = params.at("user");
  mConnection.password = params.at("password");
  mConnection.sslPort = std::stoi(params.at("ssl_port"));

  // Node addresses.
  registerDatalayerNode("new_position", params.at("new_position"));
  registerDatalayerNode("new_position_timestamp", params.at("new_position_timestamp"));
  registerDatalayerNode("next_pos_index", params.at("next_pos_index"));
  registerDatalayerNode("execute_movements", params.at("execute_movements"));
  registerDatalayerNode("position", params.at("position"));
  registerDatalayerNode("velocity", params.at("velocity"));
  registerDatalayerNode("status", params.at("status"));
  registerDatalayerNode("read_mode", params.at("read_mode"));
  registerDatalayerNode("set_mode", params.at("set_mode"));

  // Virtual command nodes for axis control services
  registerDatalayerNode("virtual_reset", params.at("virtual_reset"));
  registerDatalayerNode("virtual_reference", params.at("virtual_reference"));
  registerDatalayerNode("virtual_stop", params.at("virtual_stop"));

  // Error code node for reading latest error
  registerDatalayerNode("error_code", params.at("error_code"));

  // Hardware settings.
  mPositionSettings.initialIndex = static_cast<uint16_t>(std::stoi(params.at("initial_position_index")));
  mPositionSettings.nextPositionIndex = mPositionSettings.initialIndex;
  mPositionSettings.maxPositionIndices = static_cast<uint16_t>(std::stoi(params.at("max_position_indices")));
  mPositionSettings.newPositionsReceivedCount = 0U;
  mPositionSettings.executeMovementsOnIndex =
    static_cast<uint16_t>(std::stoi(params.at("execute_movements_on_next_index")));

  mExpectedDelayBetweenCommandsMs = static_cast<uint32_t>(1000.F / std::stof(params.at("update_frequency_hz")));

  // Position tolerance for checking if axis reached target position (default 0.01mm)
  if (params.find("position_tolerance_mm") != params.end()) {
    mPositionToleranceMm = std::stod(params.at("position_tolerance_mm"));
  }

  // Logger.
  setLogLevel(params.at("log_level"));

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Initialize resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_configure(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Configure resource STARTED...");
  auto connectionResult = connect();
  if (connectionResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    return connectionResult;
  }
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Configure resource FINISHED.");
  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn Resource::on_activate(const rclcpp_lifecycle::State&)
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Activate resource STARTED...");

  waitUntilRequiredNodesAreValid();

  auto axisStateResult = checkAxisState(AxisState::STANDSTILL);
  if (axisStateResult == hardware_interface::CallbackReturn::SUCCESS) {
    RCLCPP_INFO(rclcpp::get_logger(LINROB), "Axis is in STANDSTILL state - proceeding with full activation");
    mAxisReadyForOperation = true;

    // Switch to AUTO_EXTERNAL mode since axis is ready
    if (!switchToAutoExternalMode()) {
      return hardware_interface::CallbackReturn::ERROR;
    }
  } else {
    RCLCPP_WARN(rclcpp::get_logger(LINROB), "Axis is not in STANDSTILL state yet");
    mAxisReadyForOperation = false;
    mLastAxisStateCheck = std::chrono::steady_clock::now();
  }

  // Only reset PLC if axis is ready for operation
  if (mAxisReadyForOperation) {
    auto resetResult = resetPlcBufferAndIndex();
    if (!resetResult)
    {
      RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to reset PLC buffer and index.");
      return hardware_interface::CallbackReturn::ERROR;
    }
  }

  mPositionSettings.newPositionsReceivedCount = 0U;
  mMovementExecutionStopped = true;

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Resource activation FINISHED. Axis ready for operation: %s",
             mAxisReadyForOperation ? "YES" : "NO");
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

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting %ld seconds for PLC to change operation mode...", kSetModeSleep.count());
  std::this_thread::sleep_for(kSetModeSleep);

  auto systemModeResult = waitForSystemMode("MANUAL", kStateWaitTimeout);
  if (systemModeResult != hardware_interface::CallbackReturn::SUCCESS)
  {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to switch to MANUAL mode during deactivation");
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

void Resource::disconnect()
{
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Disconnecting..");
  if (mClient)
  {
    mClient.reset();
  }
  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Disconnected.");
}

hardware_interface::return_type Resource::write(const rclcpp::Time& time, const rclcpp::Duration&)
{
  // Handle virtual commands
  processVirtualCommands();

  // Check if axis is ready for operation
  if (!mAxisReadyForOperation)
  {
    checkAxisReadiness();
    if (!mAxisReadyForOperation)
    {
      return hardware_interface::return_type::OK;
    }
  }

  // Check if new position command is received
  if (mPositionCommand == mLastPositionCommand)
  {
    // No new position received, check if we should stop movement execution
    if (mPositionSettings.newPositionsReceivedCount > 0)
    {
      auto expectedDurationBetweenCommands = rclcpp::Duration(std::chrono::milliseconds(mExpectedDelayBetweenCommandsMs));
      bool newCommandsExpected = time - mLastPositionCommandTime < expectedDurationBetweenCommands;

      if (!newCommandsExpected && !mMovementExecutionStopped)
      {
        // Check if axis is in STANDBY state and within tolerance of last target position
        auto statusUpdateResult = updateDataFromNode("status", comm::datalayer::VariantType::ARRAY_OF_INT32);
        if (!statusUpdateResult)
        {
          return hardware_interface::return_type::ERROR;
        }

        auto& statusData = mConnection.datalayerNodeMap.at("status");
        auto axisStatus = static_cast<AxisState>(variantDataToVector<int>(statusData.second)[0U]);

        double currentPosition = mState.at("position");
        double positionError = std::abs(currentPosition - mLastPositionCommand); // Convert to mm for comparison
        RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
                     "Target position is %.8f mm at tolerance: %.8f mm", mLastPositionCommand, mPositionToleranceMm);
        RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
                     "Current position is %.8f mm, error is %.8f mm", currentPosition, positionError);

        //if (axisStatus == AxisState::STANDSTILL && positionError <= mPositionToleranceMm)
        if (positionError <= mPositionToleranceMm)
        {
          mPositionSettings.newPositionsReceivedCount = 0U;
          if (!writeToDatalayerNode("execute_movements", false))
            return hardware_interface::return_type::ERROR;

          mMovementExecutionStopped = true;
          RCLCPP_INFO(rclcpp::get_logger(LINROB),
                     "Movement execution stopped. Axis reached target position %.8f mm (tolerance: %.8f mm)",
                     mLastPositionCommand, mPositionToleranceMm);
        }
        else
        {
          RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
                      "Waiting for axis to reach target. State: %u, Position: %.8f mm, Target: %.8f mm, Error: %.8f mm",
                      static_cast<unsigned int>(axisStatus), currentPosition, mLastPositionCommand, positionError);
        }
      }
    }
    return hardware_interface::return_type::OK;
  }

  // New position received - calculate time difference since last position command
  double timeDiffMs = 0.0;
  if (mPositionSettings.newPositionsReceivedCount > 0)
  {
    auto duration = time - mLastPositionCommandTime;
    timeDiffMs = duration.seconds() * 1000.0;
  }

  // Update tracking variables
  mLastPositionCommand = mPositionCommand;
  mLastPositionCommandTime = time;
  ++mPositionSettings.newPositionsReceivedCount;

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
  // Update timestamp buffer with time difference
  mAxisTargetPositionTimestampExt[pos] = static_cast<float>(timeDiffMs);

  // Send updated buffers and index to the PLC
  if (!writeToDatalayerNode("new_position", mAxisTargetPositionsExt))
    return hardware_interface::return_type::ERROR;
  if (!writeToDatalayerNode("new_position_timestamp", mAxisTargetPositionTimestampExt))
    return hardware_interface::return_type::ERROR;
  if (!writeToDatalayerNode("next_pos_index", mPositionSettings.nextPositionIndex))
    return hardware_interface::return_type::ERROR;

  // Start movement if required number of positions have been received
  if (mPositionSettings.newPositionsReceivedCount >= mPositionSettings.executeMovementsOnIndex)
  {
    if (!writeToDatalayerNode("execute_movements", true))
      return hardware_interface::return_type::ERROR;

    mMovementExecutionStopped = false;
  }

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type Resource::read(const rclcpp::Time&, const rclcpp::Duration&)
{
  if (!mAxisReadyForOperation)
  {
    checkAxisReadiness();
    if (!mAxisReadyForOperation)
    {
      return hardware_interface::return_type::OK;
    }
  }

  auto updateResult = true;
  // Update current system and axis info.
  updateResult &= updateDataFromNode("status", comm::datalayer::VariantType::ARRAY_OF_INT32);
  updateResult &= updateDataFromNode("read_mode", comm::datalayer::VariantType::STRING);

  // Update current position and velocity info.
  updateResult &= updateDataFromNode("position", comm::datalayer::VariantType::FLOAT64);
  updateResult &= updateDataFromNode("velocity", comm::datalayer::VariantType::FLOAT64);

  // Update error code
  updateResult &= updateDataFromNode("error_code", comm::datalayer::VariantType::UINT32);

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
  state_interfaces.emplace_back("joint_1", "error_code", &mState.at("error_code"));
  state_interfaces.emplace_back("joint_1", "axis_ready", &mState.at("axis_ready"));
  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> Resource::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.emplace_back("joint_1", "position", &mPositionCommand);
  command_interfaces.emplace_back("joint_1", "virtual_reset", &mVirtualResetCommand);
  command_interfaces.emplace_back("joint_1", "virtual_reference", &mVirtualReferenceCommand);
  command_interfaces.emplace_back("joint_1", "virtual_stop", &mVirtualStopCommand);
  return command_interfaces;
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

void Resource::updateState()
{
  mState.at("position") = *reinterpret_cast<double*>(mConnection.datalayerNodeMap.at("position").second.getData());
  mState.at("velocity") = *reinterpret_cast<double*>(mConnection.datalayerNodeMap.at("velocity").second.getData());

  uint32_t errorCode = getLatestErrorCode();
  mState.at("error_code") = static_cast<double>(errorCode);
  if (errorCode != 0) {
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Error code: 0x%08X (%u)", errorCode, errorCode);
  }

  mState.at("axis_ready") = mAxisReadyForOperation ? 1.0 : 0.0;
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
    updateResult &= updateDataFromNode("position", comm::datalayer::VariantType::FLOAT64);
    updateResult &= updateDataFromNode("velocity", comm::datalayer::VariantType::FLOAT64);
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB),
                 "Update final result value: %s; If all valid: 1",
                 std::to_string(updateResult).c_str());

    wait = !updateResult;
    RCLCPP_DEBUG(rclcpp::get_logger(LINROB), "Wait value: %s", std::to_string(wait).c_str());
  }
}

bool Resource::resetPlcBufferAndIndex() {
  resetAxisTargetPositionsExt();
  resetAxisTargetPositionTimestampExt();
  mPositionSettings.nextPositionIndex = 1;
  auto newPositionWriteResult = writeToDatalayerNode("new_position", mAxisTargetPositionsExt);
  auto newTimestampWriteResult = writeToDatalayerNode("new_position_timestamp", mAxisTargetPositionTimestampExt);
  auto nextPosIndexWriteResult = writeToDatalayerNode("next_pos_index", mPositionSettings.nextPositionIndex);
  return newPositionWriteResult && newTimestampWriteResult && nextPosIndexWriteResult;
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

void Resource::processVirtualCommands()
{
  // virtual_reset
  if (mVirtualResetCommand > 0.5 && !mResetCommandExecuted)
  {
    bool result = writeToDatalayerNode("virtual_reset", true);
    if (result)
    {
      RCLCPP_INFO(rclcpp::get_logger(LINROB), "Virtual reset command sent to PLC");
      mResetCommandExecuted = true;
    }
    else
    {
      RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to send virtual reset command");
    }
  }
  else if (mVirtualResetCommand <= 0.5)
  {
    mResetCommandExecuted = false;
  }

  // virtual_reference
  if (mVirtualReferenceCommand > 0.5 && !mReferenceCommandExecuted)
  {
    bool result = writeToDatalayerNode("virtual_reference", true);
    if (result)
    {
      RCLCPP_INFO(rclcpp::get_logger(LINROB), "Virtual reference command sent to PLC");
      mReferenceCommandExecuted = true;
    }
    else
    {
      RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to send virtual reference command");
    }
  }
  else if (mVirtualReferenceCommand <= 0.5)
  {
    mReferenceCommandExecuted = false;
  }

  // virtual_stop
  if (mVirtualStopCommand > 0.5 && !mStopCommandExecuted)
  {
    bool result = writeToDatalayerNode("virtual_stop", true);
    if (result)
    {
      RCLCPP_INFO(rclcpp::get_logger(LINROB), "Virtual stop command sent to PLC");
      mStopCommandExecuted = true;
    }
    else
    {
      RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to send virtual stop command");
    }
  }
  else if (mVirtualStopCommand <= 0.5)
  {
    mStopCommandExecuted = false;
  }
}

uint32_t Resource::getLatestErrorCode()
{
  auto& errorData = mConnection.datalayerNodeMap.at("error_code");
  if (errorData.second.getType() == comm::datalayer::VariantType::UINT32)
  {
    mLatestErrorCode = *reinterpret_cast<uint32_t*>(errorData.second.getData());
  }
  return mLatestErrorCode;
}

bool Resource::switchToAutoExternalMode()
{
  // Set system mode to AUTO_EXTERNAL
  auto setSystemModeResult = writeToDatalayerNode("set_mode", static_cast<uint8_t>(Mode::AUTO_EXTERNAL));
  if (!setSystemModeResult) {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to set AUTO_EXTERNAL mode");
    return false;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Waiting %ld seconds for PLC to change operation mode...", kSetModeSleep.count());
  std::this_thread::sleep_for(kSetModeSleep);

  auto systemModeResult = waitForSystemMode("AUTO_EXTERNAL", kStateWaitTimeout);
  if (systemModeResult != hardware_interface::CallbackReturn::SUCCESS) {
    RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to switch to AUTO_EXTERNAL mode");
    return false;
  }

  RCLCPP_INFO(rclcpp::get_logger(LINROB), "Successfully switched to AUTO_EXTERNAL mode");
  return true;
}

void Resource::checkAxisReadiness()
{
  // Only check periodically to avoid excessive polling
  auto now = std::chrono::steady_clock::now();
  if (now - mLastAxisStateCheck < kAxisReadinessCheckInterval) {
    return;
  }

  mLastAxisStateCheck = now;

  // Check if axis has reached STANDSTILL state
  if (!mAxisReadyForOperation) {
    auto axisStateResult = checkAxisState(AxisState::STANDSTILL);
    if (axisStateResult == hardware_interface::CallbackReturn::SUCCESS) {
      RCLCPP_INFO(rclcpp::get_logger(LINROB), "Axis has reached STANDSTILL state - enabling operations and switching to AUTO_EXTERNAL mode");

      // Switch to AUTO_EXTERNAL mode now that axis is ready
      if (!switchToAutoExternalMode()) {
        return;
      }

      mAxisReadyForOperation = true;

      // Now that axis is ready and mode is set, reset PLC buffer and index
      auto resetResult = resetPlcBufferAndIndex();
      if (!resetResult) {
        RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Failed to reset PLC buffer and index after axis became ready");
        mAxisReadyForOperation = false; // Reset to prevent operations until next successful check
      }
    } else if (axisStateResult == hardware_interface::CallbackReturn::ERROR) {
      RCLCPP_ERROR(rclcpp::get_logger(LINROB), "Axis is in ERROR state - operations will remain disabled");
    }
  }
}

}  // namespace linrob

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(linrob::Resource, hardware_interface::SystemInterface)
