// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from linrob_axis:srv/StartMotion.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__START_MOTION__TRAITS_HPP_
#define LINROB_AXIS__SRV__DETAIL__START_MOTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "linrob_axis/srv/detail/start_motion__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace linrob_axis
{

namespace srv
{

inline void to_flow_style_yaml(
  const StartMotion_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: start
  {
    out << "start: ";
    rosidl_generator_traits::value_to_yaml(msg.start, out);
    out << ", ";
  }

  // member: target_position
  {
    out << "target_position: ";
    rosidl_generator_traits::value_to_yaml(msg.target_position, out);
    out << ", ";
  }

  // member: velocity
  {
    out << "velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.velocity, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const StartMotion_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: start
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "start: ";
    rosidl_generator_traits::value_to_yaml(msg.start, out);
    out << "\n";
  }

  // member: target_position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_position: ";
    rosidl_generator_traits::value_to_yaml(msg.target_position, out);
    out << "\n";
  }

  // member: velocity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "velocity: ";
    rosidl_generator_traits::value_to_yaml(msg.velocity, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const StartMotion_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace linrob_axis

namespace rosidl_generator_traits
{

[[deprecated("use linrob_axis::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const linrob_axis::srv::StartMotion_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  linrob_axis::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use linrob_axis::srv::to_yaml() instead")]]
inline std::string to_yaml(const linrob_axis::srv::StartMotion_Request & msg)
{
  return linrob_axis::srv::to_yaml(msg);
}

template<>
inline const char * data_type<linrob_axis::srv::StartMotion_Request>()
{
  return "linrob_axis::srv::StartMotion_Request";
}

template<>
inline const char * name<linrob_axis::srv::StartMotion_Request>()
{
  return "linrob_axis/srv/StartMotion_Request";
}

template<>
struct has_fixed_size<linrob_axis::srv::StartMotion_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<linrob_axis::srv::StartMotion_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<linrob_axis::srv::StartMotion_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace linrob_axis
{

namespace srv
{

inline void to_flow_style_yaml(
  const StartMotion_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const StartMotion_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const StartMotion_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace linrob_axis

namespace rosidl_generator_traits
{

[[deprecated("use linrob_axis::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const linrob_axis::srv::StartMotion_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  linrob_axis::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use linrob_axis::srv::to_yaml() instead")]]
inline std::string to_yaml(const linrob_axis::srv::StartMotion_Response & msg)
{
  return linrob_axis::srv::to_yaml(msg);
}

template<>
inline const char * data_type<linrob_axis::srv::StartMotion_Response>()
{
  return "linrob_axis::srv::StartMotion_Response";
}

template<>
inline const char * name<linrob_axis::srv::StartMotion_Response>()
{
  return "linrob_axis/srv/StartMotion_Response";
}

template<>
struct has_fixed_size<linrob_axis::srv::StartMotion_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<linrob_axis::srv::StartMotion_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<linrob_axis::srv::StartMotion_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<linrob_axis::srv::StartMotion>()
{
  return "linrob_axis::srv::StartMotion";
}

template<>
inline const char * name<linrob_axis::srv::StartMotion>()
{
  return "linrob_axis/srv/StartMotion";
}

template<>
struct has_fixed_size<linrob_axis::srv::StartMotion>
  : std::integral_constant<
    bool,
    has_fixed_size<linrob_axis::srv::StartMotion_Request>::value &&
    has_fixed_size<linrob_axis::srv::StartMotion_Response>::value
  >
{
};

template<>
struct has_bounded_size<linrob_axis::srv::StartMotion>
  : std::integral_constant<
    bool,
    has_bounded_size<linrob_axis::srv::StartMotion_Request>::value &&
    has_bounded_size<linrob_axis::srv::StartMotion_Response>::value
  >
{
};

template<>
struct is_service<linrob_axis::srv::StartMotion>
  : std::true_type
{
};

template<>
struct is_service_request<linrob_axis::srv::StartMotion_Request>
  : std::true_type
{
};

template<>
struct is_service_response<linrob_axis::srv::StartMotion_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // LINROB_AXIS__SRV__DETAIL__START_MOTION__TRAITS_HPP_
