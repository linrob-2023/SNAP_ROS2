// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from linrob_axis:srv/StartMotion.idl
// generated code does not contain a copyright notice
#include "linrob_axis/srv/detail/start_motion__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
linrob_axis__srv__StartMotion_Request__init(linrob_axis__srv__StartMotion_Request * msg)
{
  if (!msg) {
    return false;
  }
  // start
  // target_position
  // velocity
  return true;
}

void
linrob_axis__srv__StartMotion_Request__fini(linrob_axis__srv__StartMotion_Request * msg)
{
  if (!msg) {
    return;
  }
  // start
  // target_position
  // velocity
}

bool
linrob_axis__srv__StartMotion_Request__are_equal(const linrob_axis__srv__StartMotion_Request * lhs, const linrob_axis__srv__StartMotion_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // start
  if (lhs->start != rhs->start) {
    return false;
  }
  // target_position
  if (lhs->target_position != rhs->target_position) {
    return false;
  }
  // velocity
  if (lhs->velocity != rhs->velocity) {
    return false;
  }
  return true;
}

bool
linrob_axis__srv__StartMotion_Request__copy(
  const linrob_axis__srv__StartMotion_Request * input,
  linrob_axis__srv__StartMotion_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // start
  output->start = input->start;
  // target_position
  output->target_position = input->target_position;
  // velocity
  output->velocity = input->velocity;
  return true;
}

linrob_axis__srv__StartMotion_Request *
linrob_axis__srv__StartMotion_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Request * msg = (linrob_axis__srv__StartMotion_Request *)allocator.allocate(sizeof(linrob_axis__srv__StartMotion_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(linrob_axis__srv__StartMotion_Request));
  bool success = linrob_axis__srv__StartMotion_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
linrob_axis__srv__StartMotion_Request__destroy(linrob_axis__srv__StartMotion_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    linrob_axis__srv__StartMotion_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
linrob_axis__srv__StartMotion_Request__Sequence__init(linrob_axis__srv__StartMotion_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Request * data = NULL;

  if (size) {
    data = (linrob_axis__srv__StartMotion_Request *)allocator.zero_allocate(size, sizeof(linrob_axis__srv__StartMotion_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = linrob_axis__srv__StartMotion_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        linrob_axis__srv__StartMotion_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
linrob_axis__srv__StartMotion_Request__Sequence__fini(linrob_axis__srv__StartMotion_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      linrob_axis__srv__StartMotion_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

linrob_axis__srv__StartMotion_Request__Sequence *
linrob_axis__srv__StartMotion_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Request__Sequence * array = (linrob_axis__srv__StartMotion_Request__Sequence *)allocator.allocate(sizeof(linrob_axis__srv__StartMotion_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = linrob_axis__srv__StartMotion_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
linrob_axis__srv__StartMotion_Request__Sequence__destroy(linrob_axis__srv__StartMotion_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    linrob_axis__srv__StartMotion_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
linrob_axis__srv__StartMotion_Request__Sequence__are_equal(const linrob_axis__srv__StartMotion_Request__Sequence * lhs, const linrob_axis__srv__StartMotion_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!linrob_axis__srv__StartMotion_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
linrob_axis__srv__StartMotion_Request__Sequence__copy(
  const linrob_axis__srv__StartMotion_Request__Sequence * input,
  linrob_axis__srv__StartMotion_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(linrob_axis__srv__StartMotion_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    linrob_axis__srv__StartMotion_Request * data =
      (linrob_axis__srv__StartMotion_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!linrob_axis__srv__StartMotion_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          linrob_axis__srv__StartMotion_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!linrob_axis__srv__StartMotion_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

bool
linrob_axis__srv__StartMotion_Response__init(linrob_axis__srv__StartMotion_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    linrob_axis__srv__StartMotion_Response__fini(msg);
    return false;
  }
  return true;
}

void
linrob_axis__srv__StartMotion_Response__fini(linrob_axis__srv__StartMotion_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
linrob_axis__srv__StartMotion_Response__are_equal(const linrob_axis__srv__StartMotion_Response * lhs, const linrob_axis__srv__StartMotion_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
linrob_axis__srv__StartMotion_Response__copy(
  const linrob_axis__srv__StartMotion_Response * input,
  linrob_axis__srv__StartMotion_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

linrob_axis__srv__StartMotion_Response *
linrob_axis__srv__StartMotion_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Response * msg = (linrob_axis__srv__StartMotion_Response *)allocator.allocate(sizeof(linrob_axis__srv__StartMotion_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(linrob_axis__srv__StartMotion_Response));
  bool success = linrob_axis__srv__StartMotion_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
linrob_axis__srv__StartMotion_Response__destroy(linrob_axis__srv__StartMotion_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    linrob_axis__srv__StartMotion_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
linrob_axis__srv__StartMotion_Response__Sequence__init(linrob_axis__srv__StartMotion_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Response * data = NULL;

  if (size) {
    data = (linrob_axis__srv__StartMotion_Response *)allocator.zero_allocate(size, sizeof(linrob_axis__srv__StartMotion_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = linrob_axis__srv__StartMotion_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        linrob_axis__srv__StartMotion_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
linrob_axis__srv__StartMotion_Response__Sequence__fini(linrob_axis__srv__StartMotion_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      linrob_axis__srv__StartMotion_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

linrob_axis__srv__StartMotion_Response__Sequence *
linrob_axis__srv__StartMotion_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  linrob_axis__srv__StartMotion_Response__Sequence * array = (linrob_axis__srv__StartMotion_Response__Sequence *)allocator.allocate(sizeof(linrob_axis__srv__StartMotion_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = linrob_axis__srv__StartMotion_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
linrob_axis__srv__StartMotion_Response__Sequence__destroy(linrob_axis__srv__StartMotion_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    linrob_axis__srv__StartMotion_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
linrob_axis__srv__StartMotion_Response__Sequence__are_equal(const linrob_axis__srv__StartMotion_Response__Sequence * lhs, const linrob_axis__srv__StartMotion_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!linrob_axis__srv__StartMotion_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
linrob_axis__srv__StartMotion_Response__Sequence__copy(
  const linrob_axis__srv__StartMotion_Response__Sequence * input,
  linrob_axis__srv__StartMotion_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(linrob_axis__srv__StartMotion_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    linrob_axis__srv__StartMotion_Response * data =
      (linrob_axis__srv__StartMotion_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!linrob_axis__srv__StartMotion_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          linrob_axis__srv__StartMotion_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!linrob_axis__srv__StartMotion_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
