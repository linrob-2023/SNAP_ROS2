import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64MultiArray


class SCurvePublisher(Node):
    def __init__(self):
        super().__init__('s_curve_publisher')

        # --- Motion limits ---
        self.v_max = 400.0      # mm/s
        self.a_max = 2000.0     # mm/s²
        self.j_max = 10000.0    # mm/s³
        self.dt_internal = 0.002  # integration step = 2 ms
        self.dt_publish = 0.020   # output step = 20 ms

        # --- Move request ---
        self.start_pos = 120.0
        self.target_pos = 80.0

        # Precompute trajectory (sampled at 2 ms internally)
        self.trajectory = self.generate_scurve(self.start_pos, self.target_pos)

        # Select every 20 ms sample
        stride = int(self.dt_publish / self.dt_internal)
        self.trajectory = self.trajectory[::stride]

        self.index = 0

        self.publisher_ = self.create_publisher(Float64MultiArray,
                                                '/position_controller/commands',
                                                10)
        self.timer = self.create_timer(self.dt_publish, self.timer_callback)

        self.get_logger().info(f"Trajectory with {len(self.trajectory)} samples ready.")

    def generate_scurve(self, x0, x1):
        """Generate jerk-limited S-curve trajectory (sampled at dt_internal)."""
        d = x1 - x0
        direction = 1.0 if d >= 0 else -1.0
        distance = abs(d)

        # Simple symmetric profile (long enough move to reach vmax)
        t_acc = self.v_max / self.a_max
        t_flat = max((distance - self.v_max * t_acc), 0.0) / self.v_max
        total_time = 2 * t_acc + t_flat

        n_samples = int(total_time / self.dt_internal) + 1

        pos = 0.0
        vel = 0.0
        acc = 0.0
        trajectory = []

        for i in range(n_samples):
            trajectory.append(x0 + direction * pos)

            t = i * self.dt_internal
            if t < t_acc:
                jerk = self.j_max
            elif t < t_acc + t_flat:
                jerk = 0.0
                acc = 0.0
            elif t < 2 * t_acc + t_flat:
                jerk = -self.j_max
            else:
                jerk = 0.0
                acc = 0.0
                vel = 0.0

            acc += jerk * self.dt_internal
            acc = max(min(acc, self.a_max), -self.a_max)

            vel += acc * self.dt_internal
            vel = max(min(vel, self.v_max), 0.0)

            pos += vel * self.dt_internal

        return trajectory

    def timer_callback(self):
        if self.index >= len(self.trajectory):
            return

        pos = self.trajectory[self.index]
        msg = Float64MultiArray()
        msg.data = [pos]
        self.publisher_.publish(msg)

        if self.index == len(self.trajectory) - 1:
            self.get_logger().info(f"Final position sent: {pos:.3f} mm")

        self.index += 1


def main(args=None):
    rclpy.init(args=args)
    node = SCurvePublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
