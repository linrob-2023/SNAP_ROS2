FROM ros:humble-ros-base

RUN apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    ros-humble-ros-gz-sim \
    ros-humble-ros-gz-bridge \
    ros-humble-gz-ros2-control \
    ros-humble-robot-state-publisher \
    ros-humble-ros2-control \
    ros-humble-ros2-controllers \
    ros-humble-position-controllers \
    ros-humble-joint-state-broadcaster \
    ros-humble-xacro \
    python3-rosdep \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /ros2_ws

COPY src/linrob_sim_cart ./src/linrob_sim_cart

RUN rosdep update \
 && rosdep install --from-paths src --ignore-src -r -y

RUN . /opt/ros/humble/setup.sh && colcon build --packages-select linrob_sim_cart

ENTRYPOINT ["/bin/bash", "-c", "source /opt/ros/humble/setup.bash && source /ros2_ws/install/setup.bash && exec \"$@\"", "--"]

CMD ["ros2", "launch", "linrob_sim_cart", "sim_cart.headless.launch.py"]
