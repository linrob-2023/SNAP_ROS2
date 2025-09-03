// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from linrob_axis:srv/ReferenceAxis.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__STRUCT_HPP_
#define LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__linrob_axis__srv__ReferenceAxis_Request __attribute__((deprecated))
#else
# define DEPRECATED__linrob_axis__srv__ReferenceAxis_Request __declspec(deprecated)
#endif

namespace linrob_axis
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ReferenceAxis_Request_
{
  using Type = ReferenceAxis_Request_<ContainerAllocator>;

  explicit ReferenceAxis_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit ReferenceAxis_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__linrob_axis__srv__ReferenceAxis_Request
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__linrob_axis__srv__ReferenceAxis_Request
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ReferenceAxis_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const ReferenceAxis_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ReferenceAxis_Request_

// alias to use template instance with default allocator
using ReferenceAxis_Request =
  linrob_axis::srv::ReferenceAxis_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace linrob_axis


#ifndef _WIN32
# define DEPRECATED__linrob_axis__srv__ReferenceAxis_Response __attribute__((deprecated))
#else
# define DEPRECATED__linrob_axis__srv__ReferenceAxis_Response __declspec(deprecated)
#endif

namespace linrob_axis
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct ReferenceAxis_Response_
{
  using Type = ReferenceAxis_Response_<ContainerAllocator>;

  explicit ReferenceAxis_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit ReferenceAxis_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__linrob_axis__srv__ReferenceAxis_Response
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__linrob_axis__srv__ReferenceAxis_Response
    std::shared_ptr<linrob_axis::srv::ReferenceAxis_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ReferenceAxis_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const ReferenceAxis_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ReferenceAxis_Response_

// alias to use template instance with default allocator
using ReferenceAxis_Response =
  linrob_axis::srv::ReferenceAxis_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace linrob_axis

namespace linrob_axis
{

namespace srv
{

struct ReferenceAxis
{
  using Request = linrob_axis::srv::ReferenceAxis_Request;
  using Response = linrob_axis::srv::ReferenceAxis_Response;
};

}  // namespace srv

}  // namespace linrob_axis

#endif  // LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__STRUCT_HPP_
