# Linrob

## Package: linrob_axis

### Summary

This package contains logic to start:

- Position controller
- Joint State publisher
- Axis controller
- Resource plugin that establishes connection between ctrlX datalayer and ROS2 topics. /position_controller/commands for sending commands to the hardware and /joint_states for reading current hardware state.

### Configurations

All Datalayer nodes addresses and other hardware interface settings should be defined in the robot description urdf file at:
./description/robot_description.xacro.urdf

### Building snap app

Navigate to:

```bash
cd path/to/Linrob/ros2_ws
```

Run script:

```bash
sudo ./build-snap-amd64.sh
```

This script will build all ros2 packages and build snap container.
Snap build configured to start launch file when .snap file is started.

NOTE: Package snapcraft.yaml configured not to include ros2 base and control packages, but it depends on them. Make sure snap application with those packages is added to your system.

### Local build

Navigate to:

```bash
cd path/to/Linrob/ros2_ws
```

Build:

```bash
colcon build --packages-select linrob_axis
```

Launch package:

```bash
ros2 launch linrob_axis start.launch.py
```

### Resource Activation & Deactivation

By default, after startup and launching, the joint_state_broadcaster is **active** and both the position_controller and axis_controller are **inactive** until you activate them using ROS 2 controller commands.

#### Activation

To activate the position controller, run:
```sh
ros2 control switch_controllers --activate position_controller
```

To activate the axis controller, run:
```sh
ros2 control switch_controllers --activate axis_controller
```

To activate both controllers, run:
```sh
ros2 control switch_controllers --activate position_controller axis_controller
```

To activate the hardware component, run:
```sh
ros2 control set_hardware_component_state linrob_hw active
```

#### Deactivation

To deactivate the position controller, run:
```sh
ros2 control switch_controllers --deactivate position_controller
```

To deactivate the axis controller, run:
```sh
ros2 control switch_controllers --deactivate axis_controller
```

To deactivate both controllers, run:
```sh
ros2 control switch_controllers --deactivate position_controller axis_controller
```

To deactivate the hardware component, run:
```sh
ros2 control set_hardware_component_state linrob_hw inactive
```


#### Check Controller State

To verify the controller state:
```sh
ros2 control list_controllers
```

### Axis Control Services

The package provides ROS2 services for controlling the PLC axis:

- **ResetAxis** (`/reset_axis`) - Resets the axis and acknowledges errors
```sh
ros2 service call /axis_controller/reset_axis linrob_axis/srv/ResetAxis "{}"
```
- **ReferenceAxis** (`/reference_axis`) - Starts a referencing/homing process
```sh
ros2 service call /axis_controller/reference_axis linrob_axis/srv/ReferenceAxis "{}"
```
- **StopAxis** (`/stop_axis`) - Stops active movement immediately
```sh
ros2 service call /axis_controller/stop_axis linrob_axis/srv/StopAxis "{}"
```
- **StartMotion** (`/start_motion`) - Starts a motion command to a target position (no arrival check)
```sh
ros2 service call /axis_controller/start_motion linrob_axis/srv/StartMotion '{start: true, target_position: 3}'
```
  - `start` (bool): Set to true to trigger the motion command
  - `target_position` (uint8): Target position index or identifier



Error codes are continuously monitored and published on the `/error_code` topic.


## Package: linrob_sim_cart

### Summary

This package provides a simulation environment for the Linrob axis using Gazebo and ROS 2 Control.

### Configurations

All simulation and controller parameters should be defined in the robot description URDF file at:
./description/sim_cart.xacro.urdf

### Local Build

Navigate to:

```bash
cd path/to/Linrob/ros2_ws
```

Build:

```bash
colcon build --packages-select linrob_sim_cart
```

### Launch Simulation

To launch the simulation, run:

```bash
ros2 launch linrob_sim_cart sim_cart.launch.py
```

### Resource Activation & Deactivation

By default, after startup and launching, the joint_state_broadcaster is **active** and the position_controller is **inactive** until you activate it using ROS 2 controller commands.

#### Activation

To activate the position controller, run:
```sh
ros2 control switch_controllers --activate position_controller
```

#### Deactivation

To deactivate the position controller, run:
```sh
ros2 control switch_controllers --deactivate position_controller
```

#### Check Controller State

To verify the controller state:
```sh
ros2 control list_controllers
```

## Running the Simulation in Docker

**Note:** The following commands should be run from your `ros2_ws` directory.

### Build the Docker Image

```bash
docker build -t linrob_sim_cart -f src/linrob_sim_cart/Dockerfile .
```

### Run the Simulation (Headless/No GUI)

```bash
docker run --rm linrob_sim_cart
```

This will launch the simulation in headless mode.
The joint_state_broadcaster is loaded as active by default; the position_controller is inactive until activated using ROS 2 control commands.

#### Interacting with the Running Container

To list or activate controllers after the simulation is running, open a new terminal and enter the container:

```bash
docker exec -it $(docker ps -q -f ancestor=linrob_sim_cart) bash
source /opt/ros/humble/setup.bash
ros2 control list_controllers
ros2 control switch_controllers --activate position_controller
```

## ros2-base-humble-deb

This directory is copied from [here](https://github.com/boschrexroth/ctrlx-automation-sdk-ros2/tree/main/ros2-base-humble-deb) and slightly modified to have required ros2 base and ros2 control packages.
