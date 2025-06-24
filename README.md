# Linrob

## Package: linrob_axis

### Summary

This package contains logic to start:

- Position controller
- Joint State publisher
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

By default, after startup and launching, the resource is configured but remains **inactive** until you activate it using ROS 2 controller commands.

#### Activation

To activate the resource controller, run:
```sh
ros2 control switch_controllers --activate position_controller joint_state_broadcaster --strict
```

#### Deactivation

To deactivate the resource controller, run:
```sh
ros2 control switch_controllers --deactivate position_controller joint_state_broadcaster --strict
```

#### Check Controller State

To verify the controller state:
```sh
ros2 control list_controllers
```

## Package: linrob_command_sender

### Summary

This package is responsible for publishing commands to the server.

Starts:

- Client position controller.
- Client resource plugin that can process client commands and send them to server via command topic.

### Configurations

All plugin parameters should be specified in robot description urdf file.
Current parameters:

- command_topic - Topic where plugin command publisher will publish new commands.

### Local build

Navigate to:

```bash
cd path/to/Linrob/ros2_ws
```

Build:

```bash
colcon build --packages-select linrob_command_sender
```

Launch:

```bash
ros2 launch linrob_command_sender start.launch.py
```

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

By default, after startup and launching, the resource is configured but remains **inactive** until you activate it using ROS 2 controller commands.

#### Activation

To activate the resource controller, run:
```sh
ros2 control switch_controllers --activate position_controller joint_state_broadcaster --strict
```

#### Deactivation

To deactivate the resource controller, run:
```sh
ros2 control switch_controllers --deactivate position_controller joint_state_broadcaster --strict
```

#### Check Controller State

To verify the controller state:
```sh
ros2 control list_controllers
```

## ros2-base-humble-deb

This directory is copied from [here](https://github.com/boschrexroth/ctrlx-automation-sdk-ros2/tree/main/ros2-base-humble-deb) and slightly modified to have required ros2 base and ros2 control packages.
