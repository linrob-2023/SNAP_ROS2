// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from linrob_axis:srv/StartMotion.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__START_MOTION__STRUCT_H_
#define LINROB_AXIS__SRV__DETAIL__START_MOTION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/StartMotion in the package linrob_axis.
typedef struct linrob_axis__srv__StartMotion_Request
{
  /// Set true to trigger motion command (e.g. GVL/xVirtMove)
  bool start;
  /// Target position identifier / index
  double target_position;
  /// Target velocity for the motion
  double velocity;
} linrob_axis__srv__StartMotion_Request;

// Struct for a sequence of linrob_axis__srv__StartMotion_Request.
typedef struct linrob_axis__srv__StartMotion_Request__Sequence
{
  linrob_axis__srv__StartMotion_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} linrob_axis__srv__StartMotion_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/StartMotion in the package linrob_axis.
typedef struct linrob_axis__srv__StartMotion_Response
{
  bool success;
  rosidl_runtime_c__String message;
} linrob_axis__srv__StartMotion_Response;

// Struct for a sequence of linrob_axis__srv__StartMotion_Response.
typedef struct linrob_axis__srv__StartMotion_Response__Sequence
{
  linrob_axis__srv__StartMotion_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} linrob_axis__srv__StartMotion_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // LINROB_AXIS__SRV__DETAIL__START_MOTION__STRUCT_H_
