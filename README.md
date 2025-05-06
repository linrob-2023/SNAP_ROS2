# Linrob

## Package: linrob_axis

### Summary

This package contains logic to start:

- Position controller
- Joint State publisher
- Resource plugin that establishes connection between ctrlX datalayer and ROS2 topics. /position_controller/commands for sending commands to the hardware nad /joint_states for reading current hardware state.

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

## ros2-base-humble-deb

This directory is copied from [here](https://github.com/boschrexroth/ctrlx-automation-sdk-ros2/tree/main/ros2-base-humble-deb) and slightly modified to have required ros2 base and ros2 control packages.
