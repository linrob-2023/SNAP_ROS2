cmake_minimum_required(VERSION 3.8)
project(linrob_axis)
set(TARGET_PROJECT_NAME linrob_axis)

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

#
# Set link directories
#
MESSAGE( STATUS "Libraries directory: ${LIBRARY_DIR}")
link_directories(
  ${LIBRARY_DIR}
  ${LIBRARY_DEP_DIR}
  )

# User dependency directory
#
set (USER_DEPENDENCY_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../../ctrlX/ctrlx-automation-sdk)

SET ( PRIVATE_INCLUDE_DIRS
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${USER_DEPENDENCY_DIR}/include
  ${USER_DEPENDENCY_DIR}/include/comm.datalayer
  )

# Set target link libraries
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Define the plugin as a shared library
add_library(${TARGET_PROJECT_NAME} SHARED
  src/Resource.cpp
)

#
# Set target include directories
#
target_include_directories ( ${TARGET_PROJECT_NAME}
  PUBLIC ${PUBLIC_INCLUDE_DIRS}
  PUBLIC ${LIBRARY_INCLUDES}
	PRIVATE ${PRIVATE_INCLUDE_DIRS}
)
#
target_link_libraries(${TARGET_PROJECT_NAME} -Wl,--no-undefined)
target_link_libraries(${TARGET_PROJECT_NAME}
  libcomm_datalayer.so
  pthread
  systemd
  zmq
  ssl
  crypto
)

ament_target_dependencies(${TARGET_PROJECT_NAME}
  rclcpp
  std_msgs
  hardware_interface
  ros2_control
  pluginlib
)

pluginlib_export_plugin_description_file(hardware_interface resource_plugin.xml)


# Install shared library
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION lib
)

# Install plugin description file from root
install(
  FILES resource_plugin.xml
  DESTINATION share/${PROJECT_NAME}/
)

# Install YAML config files (if needed)
install(
  DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

# Install Launch
install(
  DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(
  DIRECTORY description/
  DESTINATION share/${PROJECT_NAME}/description
)

ament_package()
