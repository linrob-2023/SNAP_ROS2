// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from linrob_axis:srv/StartMotion.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__START_MOTION__BUILDER_HPP_
#define LINROB_AXIS__SRV__DETAIL__START_MOTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "linrob_axis/srv/detail/start_motion__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace linrob_axis
{

namespace srv
{

namespace builder
{

class Init_StartMotion_Request_velocity
{
public:
  explicit Init_StartMotion_Request_velocity(::linrob_axis::srv::StartMotion_Request & msg)
  : msg_(msg)
  {}
  ::linrob_axis::srv::StartMotion_Request velocity(::linrob_axis::srv::StartMotion_Request::_velocity_type arg)
  {
    msg_.velocity = std::move(arg);
    return std::move(msg_);
  }

private:
  ::linrob_axis::srv::StartMotion_Request msg_;
};

class Init_StartMotion_Request_target_position
{
public:
  explicit Init_StartMotion_Request_target_position(::linrob_axis::srv::StartMotion_Request & msg)
  : msg_(msg)
  {}
  Init_StartMotion_Request_velocity target_position(::linrob_axis::srv::StartMotion_Request::_target_position_type arg)
  {
    msg_.target_position = std::move(arg);
    return Init_StartMotion_Request_velocity(msg_);
  }

private:
  ::linrob_axis::srv::StartMotion_Request msg_;
};

class Init_StartMotion_Request_start
{
public:
  Init_StartMotion_Request_start()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StartMotion_Request_target_position start(::linrob_axis::srv::StartMotion_Request::_start_type arg)
  {
    msg_.start = std::move(arg);
    return Init_StartMotion_Request_target_position(msg_);
  }

private:
  ::linrob_axis::srv::StartMotion_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::linrob_axis::srv::StartMotion_Request>()
{
  return linrob_axis::srv::builder::Init_StartMotion_Request_start();
}

}  // namespace linrob_axis


namespace linrob_axis
{

namespace srv
{

namespace builder
{

class Init_StartMotion_Response_message
{
public:
  explicit Init_StartMotion_Response_message(::linrob_axis::srv::StartMotion_Response & msg)
  : msg_(msg)
  {}
  ::linrob_axis::srv::StartMotion_Response message(::linrob_axis::srv::StartMotion_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::linrob_axis::srv::StartMotion_Response msg_;
};

class Init_StartMotion_Response_success
{
public:
  Init_StartMotion_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StartMotion_Response_message success(::linrob_axis::srv::StartMotion_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_StartMotion_Response_message(msg_);
  }

private:
  ::linrob_axis::srv::StartMotion_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::linrob_axis::srv::StartMotion_Response>()
{
  return linrob_axis::srv::builder::Init_StartMotion_Response_success();
}

}  // namespace linrob_axis

#endif  // LINROB_AXIS__SRV__DETAIL__START_MOTION__BUILDER_HPP_
