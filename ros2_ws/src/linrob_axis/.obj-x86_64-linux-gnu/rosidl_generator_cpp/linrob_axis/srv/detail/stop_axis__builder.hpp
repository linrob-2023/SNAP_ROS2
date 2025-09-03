// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from linrob_axis:srv/StopAxis.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__STOP_AXIS__BUILDER_HPP_
#define LINROB_AXIS__SRV__DETAIL__STOP_AXIS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "linrob_axis/srv/detail/stop_axis__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace linrob_axis
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::linrob_axis::srv::StopAxis_Request>()
{
  return ::linrob_axis::srv::StopAxis_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace linrob_axis


namespace linrob_axis
{

namespace srv
{

namespace builder
{

class Init_StopAxis_Response_message
{
public:
  explicit Init_StopAxis_Response_message(::linrob_axis::srv::StopAxis_Response & msg)
  : msg_(msg)
  {}
  ::linrob_axis::srv::StopAxis_Response message(::linrob_axis::srv::StopAxis_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::linrob_axis::srv::StopAxis_Response msg_;
};

class Init_StopAxis_Response_success
{
public:
  Init_StopAxis_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StopAxis_Response_message success(::linrob_axis::srv::StopAxis_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_StopAxis_Response_message(msg_);
  }

private:
  ::linrob_axis::srv::StopAxis_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::linrob_axis::srv::StopAxis_Response>()
{
  return linrob_axis::srv::builder::Init_StopAxis_Response_success();
}

}  // namespace linrob_axis

#endif  // LINROB_AXIS__SRV__DETAIL__STOP_AXIS__BUILDER_HPP_
