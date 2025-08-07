import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray
import time
import random

class StreamPublisher(Node):
    def __init__(self):
        super().__init__('stream_publisher')
        self.publisher_ = self.create_publisher(Float64MultiArray, '/position_controller/commands', 10)
        self.timer = self.create_timer(0.05, self.timer_callback)  # 100ms
        self.value = 0.0  # Starting value

    def timer_callback(self):
        # Increment by ~0.5 with a bit of randomness
        self.value += 0.000002 # + random.uniform(-0.1, 0.1)

        msg = Float64MultiArray()
        msg.data = [self.value]
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = StreamPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
