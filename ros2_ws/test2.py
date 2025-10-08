import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
import math
import time

class WaveMotionPublisher(Node):
    def __init__(self):
        super().__init__('wave_motion_publisher')

        # Parameters (you could also declare these as ROS 2 params)
        self.amplitude = 150.0            # mm
        self.frequency = 0.05             # Hz
        self.max_velocity = 15        # mm/s
        self.max_acceleration = 0.10    # mm/s²
        self.control_rate = 10        # Hz

        self.publisher_ = self.create_publisher(Float64MultiArray, '/position_controller/commands', 10)
        timer_period = 1.0 / self.control_rate
        self.timer = self.create_timer(timer_period, self.timer_callback)  # 50ms
        self.start_time = time.time()

        # Internal state
        self.prev_time = self.get_clock().now().nanoseconds / 1e9
        self.last_position = 0.0
        self.last_velocity = 0.0
        self.omega = 2 * math.pi * self.frequency

        self.get_logger().info('WaveMotionPublisher initialized with parameters:')
        self.get_logger().info(f'  Amplitude: {self.amplitude} mm')
        self.get_logger().info(f'  Frequency: {self.frequency} Hz')
        self.get_logger().info(f'  Max Velocity: {self.max_velocity} mm/s')
        self.get_logger().info(f'  Max Acceleration: {self.max_acceleration} mm/s²')

    def timer_callback(self):
        now = self.get_clock().now().nanoseconds / 1e9
        dt = now - self.prev_time
        self.prev_time = now
        if dt <= 0.0:
            return
        
        t = now
        ideal_position = self.amplitude * math.sin(self.omega * t)
        ideal_velocity = self.amplitude * self.omega * math.cos(self.omega * t)
        
        # velocity limiting
        desired_velocity = ideal_velocity
        desired_velocity = max(min(desired_velocity, self.max_velocity), -self.max_velocity)

        # acceleration limiting
        delta_v = desired_velocity - self.last_velocity
        max_delta_v = self.max_acceleration * dt
        if abs(delta_v) > max_delta_v:
            delta_v = math.copysign(max_delta_v, delta_v)
        current_velocity = self.last_velocity + delta_v

        # position integration
        current_position = self.last_position + current_velocity * dt

        # save state
        self.last_position = current_position
        self.last_velocity = current_velocity

        # publish target position
        msg = Float64MultiArray()
        msg.data = [current_position]
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = WaveMotionPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
