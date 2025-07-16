from pathlib import Path

import xacro
from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

ROOT_DIR = Path(__file__).resolve().parents[1]


def generate_launch_description():

    # Get URDF via xacro
    xacro_file = Path(__file__).parents[1] / "description" / "robot_description.xacro.urdf"
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)

    robot_description = {"robot_description": doc.toxml()}
    publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[robot_description],
    )

    controllers = PathJoinSubstitution(
        [
            FindPackageShare("linrob_axis"),
            "config",
            "position_controller_inactive.yaml"
        ]
    )

    # Node to start the controller manager
    controller_manager = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[controllers],
        output="both",
        remappings=[
            ("~/robot_description", "/robot_description")
        ]
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "-c", "/controller_manager"],
    )

    controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["position_controller", "-c", "/controller_manager", "--inactive"],
    )

    axis_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["axis_controller", "-c", "/controller_manager", "--inactive"],
    )

    return LaunchDescription([
        publisher,
        controller_manager,
        controller_spawner,
        joint_state_broadcaster_spawner,
        axis_controller_spawner
    ])
