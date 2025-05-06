cmake_minimum_required(VERSION 3.8)
project(linrob_command_sender)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(ros2_control REQUIRED)
find_package(pluginlib REQUIRED)

include_directories(include)

add_library(${PROJECT_NAME} SHARED
  src/ClientResource.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  std_msgs
  hardware_interface
  ros2_control
  pluginlib
)

pluginlib_export_plugin_description_file(hardware_interface resource_plugin.xml)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)

install(
  FILES resource_plugin.xml
  DESTINATION share/${PROJECT_NAME}/config
)

install(
  DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(
  DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(
  DIRECTORY description/
  DESTINATION share/${PROJECT_NAME}/description
)


ament_package()
