#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>

// This node subscribes to the position controller output topic and republishes
// the most recent command (if changed) to a configured relay topic.

class CommandRelayNode : public rclcpp::Node {
public:
  CommandRelayNode() : Node("linrob_command_relay") {
    declare_parameter<std::string>("input_topic", "/client_position_command");
    declare_parameter<std::string>("output_topic", "/position_controller/commands");
    get_parameter("input_topic", input_topic_);
    get_parameter("output_topic", output_topic_);

    // Sub to controller command topic
    sub_ = create_subscription<std_msgs::msg::Float64MultiArray>(
      input_topic_, 10,
      [this](std_msgs::msg::Float64MultiArray::ConstSharedPtr msg){
        if (msg->data.empty()) return;
        double new_value = msg->data[0];
        if (!last_value_ || *last_value_ != new_value){
          last_value_ = new_value;
          publishRelay(new_value);
        }
      }
    );

    pub_ = create_publisher<std_msgs::msg::Float64MultiArray>(output_topic_, 10);

    RCLCPP_INFO(get_logger(), "Command relay started: %s -> %s", input_topic_.c_str(), output_topic_.c_str());
  }

private:
  void publishRelay(double value){
    std_msgs::msg::Float64MultiArray out; out.data.push_back(value); pub_->publish(out);
  }
  std::string input_topic_; std::string output_topic_;
  rclcpp::Subscription<std_msgs::msg::Float64MultiArray>::SharedPtr sub_;
  rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_;
  std::optional<double> last_value_;
};

int main(int argc, char** argv){
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CommandRelayNode>());
  rclcpp::shutdown();
  return 0;
}
