# --------------------------------------------------
# Base image: ROS 2 Humble on Ubuntu 22.04 (Jammy)
# --------------------------------------------------
FROM --platform=linux/arm64 ros:humble

# Avoid interactive tzdata / locale prompts
ENV DEBIAN_FRONTEND=noninteractive

# --------------------------------------------------
# Install base build dependencies
# --------------------------------------------------
RUN apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    python3-rosdep \
    python3-vcstool \
    build-essential \
    git \
    # ros2_control stack
    ros-humble-ros2-control \
    ros-humble-ros2-controllers \
    ros-humble-position-controllers \
    ros-humble-joint-state-broadcaster \
    ros-humble-controller-interface \
    ros-humble-hardware-interface \
    # gazebo + ros_gz
    ros-humble-ros-gz-sim \
    ros-humble-gz-ros2-control \
    && rm -rf /var/lib/apt/lists/*

# --------------------------------------------------
# Setup workspace
# --------------------------------------------------
WORKDIR /ros2_ws
COPY src/ ./src/

# --------------------------------------------------
# Install ROS dependencies via rosdep
# --------------------------------------------------
RUN rosdep init || true && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -r -y

# --------------------------------------------------
# Build workspace
# --------------------------------------------------
RUN . /opt/ros/humble/setup.sh && \
    colcon build --merge-install --cmake-args -DCMAKE_BUILD_TYPE=Release --packages-select linrob_axis

# --------------------------------------------------
# Source the overlay automatically
# --------------------------------------------------
SHELL ["/bin/bash", "-c"]
RUN echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc && \
    echo "source /ros2_ws/install/setup.bash" >> ~/.bashrc

CMD ["bash"]
