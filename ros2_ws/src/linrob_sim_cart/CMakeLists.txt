cmake_minimum_required(VERSION 3.10)
project(linrob_sim_cart)

find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(ros_gz_sim REQUIRED)
find_package(gz_ros2_control REQUIRED)
find_package(robot_state_publisher REQUIRED)
find_package(ros2_control REQUIRED)
find_package(ros2_controllers REQUIRED)
find_package(position_controllers REQUIRED)
find_package(joint_state_broadcaster REQUIRED)

install(
  DIRECTORY launch config description
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
