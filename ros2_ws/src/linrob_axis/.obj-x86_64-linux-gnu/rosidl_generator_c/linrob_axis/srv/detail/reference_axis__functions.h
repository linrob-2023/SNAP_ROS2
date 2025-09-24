// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from linrob_axis:srv/ReferenceAxis.idl
// generated code does not contain a copyright notice

#ifndef LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__FUNCTIONS_H_
#define LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "linrob_axis/msg/rosidl_generator_c__visibility_control.h"

#include "linrob_axis/srv/detail/reference_axis__struct.h"

/// Initialize srv/ReferenceAxis message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * linrob_axis__srv__ReferenceAxis_Request
 * )) before or use
 * linrob_axis__srv__ReferenceAxis_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__init(linrob_axis__srv__ReferenceAxis_Request * msg);

/// Finalize srv/ReferenceAxis message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Request__fini(linrob_axis__srv__ReferenceAxis_Request * msg);

/// Create srv/ReferenceAxis message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * linrob_axis__srv__ReferenceAxis_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
linrob_axis__srv__ReferenceAxis_Request *
linrob_axis__srv__ReferenceAxis_Request__create();

/// Destroy srv/ReferenceAxis message.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Request__destroy(linrob_axis__srv__ReferenceAxis_Request * msg);

/// Check for srv/ReferenceAxis message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__are_equal(const linrob_axis__srv__ReferenceAxis_Request * lhs, const linrob_axis__srv__ReferenceAxis_Request * rhs);

/// Copy a srv/ReferenceAxis message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__copy(
  const linrob_axis__srv__ReferenceAxis_Request * input,
  linrob_axis__srv__ReferenceAxis_Request * output);

/// Initialize array of srv/ReferenceAxis messages.
/**
 * It allocates the memory for the number of elements and calls
 * linrob_axis__srv__ReferenceAxis_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__Sequence__init(linrob_axis__srv__ReferenceAxis_Request__Sequence * array, size_t size);

/// Finalize array of srv/ReferenceAxis messages.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Request__Sequence__fini(linrob_axis__srv__ReferenceAxis_Request__Sequence * array);

/// Create array of srv/ReferenceAxis messages.
/**
 * It allocates the memory for the array and calls
 * linrob_axis__srv__ReferenceAxis_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
linrob_axis__srv__ReferenceAxis_Request__Sequence *
linrob_axis__srv__ReferenceAxis_Request__Sequence__create(size_t size);

/// Destroy array of srv/ReferenceAxis messages.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Request__Sequence__destroy(linrob_axis__srv__ReferenceAxis_Request__Sequence * array);

/// Check for srv/ReferenceAxis message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__Sequence__are_equal(const linrob_axis__srv__ReferenceAxis_Request__Sequence * lhs, const linrob_axis__srv__ReferenceAxis_Request__Sequence * rhs);

/// Copy an array of srv/ReferenceAxis messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Request__Sequence__copy(
  const linrob_axis__srv__ReferenceAxis_Request__Sequence * input,
  linrob_axis__srv__ReferenceAxis_Request__Sequence * output);

/// Initialize srv/ReferenceAxis message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * linrob_axis__srv__ReferenceAxis_Response
 * )) before or use
 * linrob_axis__srv__ReferenceAxis_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__init(linrob_axis__srv__ReferenceAxis_Response * msg);

/// Finalize srv/ReferenceAxis message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Response__fini(linrob_axis__srv__ReferenceAxis_Response * msg);

/// Create srv/ReferenceAxis message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * linrob_axis__srv__ReferenceAxis_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
linrob_axis__srv__ReferenceAxis_Response *
linrob_axis__srv__ReferenceAxis_Response__create();

/// Destroy srv/ReferenceAxis message.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Response__destroy(linrob_axis__srv__ReferenceAxis_Response * msg);

/// Check for srv/ReferenceAxis message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__are_equal(const linrob_axis__srv__ReferenceAxis_Response * lhs, const linrob_axis__srv__ReferenceAxis_Response * rhs);

/// Copy a srv/ReferenceAxis message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__copy(
  const linrob_axis__srv__ReferenceAxis_Response * input,
  linrob_axis__srv__ReferenceAxis_Response * output);

/// Initialize array of srv/ReferenceAxis messages.
/**
 * It allocates the memory for the number of elements and calls
 * linrob_axis__srv__ReferenceAxis_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__Sequence__init(linrob_axis__srv__ReferenceAxis_Response__Sequence * array, size_t size);

/// Finalize array of srv/ReferenceAxis messages.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Response__Sequence__fini(linrob_axis__srv__ReferenceAxis_Response__Sequence * array);

/// Create array of srv/ReferenceAxis messages.
/**
 * It allocates the memory for the array and calls
 * linrob_axis__srv__ReferenceAxis_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
linrob_axis__srv__ReferenceAxis_Response__Sequence *
linrob_axis__srv__ReferenceAxis_Response__Sequence__create(size_t size);

/// Destroy array of srv/ReferenceAxis messages.
/**
 * It calls
 * linrob_axis__srv__ReferenceAxis_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
void
linrob_axis__srv__ReferenceAxis_Response__Sequence__destroy(linrob_axis__srv__ReferenceAxis_Response__Sequence * array);

/// Check for srv/ReferenceAxis message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__Sequence__are_equal(const linrob_axis__srv__ReferenceAxis_Response__Sequence * lhs, const linrob_axis__srv__ReferenceAxis_Response__Sequence * rhs);

/// Copy an array of srv/ReferenceAxis messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_linrob_axis
bool
linrob_axis__srv__ReferenceAxis_Response__Sequence__copy(
  const linrob_axis__srv__ReferenceAxis_Response__Sequence * input,
  linrob_axis__srv__ReferenceAxis_Response__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // LINROB_AXIS__SRV__DETAIL__REFERENCE_AXIS__FUNCTIONS_H_
