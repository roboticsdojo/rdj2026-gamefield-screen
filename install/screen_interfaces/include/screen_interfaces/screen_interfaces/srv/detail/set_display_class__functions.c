// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from screen_interfaces:srv/SetDisplayClass.idl
// generated code does not contain a copyright notice
#include "screen_interfaces/srv/detail/set_display_class__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `category`
#include "rosidl_runtime_c/string_functions.h"

bool
screen_interfaces__srv__SetDisplayClass_Request__init(screen_interfaces__srv__SetDisplayClass_Request * msg)
{
  if (!msg) {
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__init(&msg->category)) {
    screen_interfaces__srv__SetDisplayClass_Request__fini(msg);
    return false;
  }
  return true;
}

void
screen_interfaces__srv__SetDisplayClass_Request__fini(screen_interfaces__srv__SetDisplayClass_Request * msg)
{
  if (!msg) {
    return;
  }
  // category
  rosidl_runtime_c__String__fini(&msg->category);
}

bool
screen_interfaces__srv__SetDisplayClass_Request__are_equal(const screen_interfaces__srv__SetDisplayClass_Request * lhs, const screen_interfaces__srv__SetDisplayClass_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->category), &(rhs->category)))
  {
    return false;
  }
  return true;
}

bool
screen_interfaces__srv__SetDisplayClass_Request__copy(
  const screen_interfaces__srv__SetDisplayClass_Request * input,
  screen_interfaces__srv__SetDisplayClass_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // category
  if (!rosidl_runtime_c__String__copy(
      &(input->category), &(output->category)))
  {
    return false;
  }
  return true;
}

screen_interfaces__srv__SetDisplayClass_Request *
screen_interfaces__srv__SetDisplayClass_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Request * msg = (screen_interfaces__srv__SetDisplayClass_Request *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(screen_interfaces__srv__SetDisplayClass_Request));
  bool success = screen_interfaces__srv__SetDisplayClass_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
screen_interfaces__srv__SetDisplayClass_Request__destroy(screen_interfaces__srv__SetDisplayClass_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    screen_interfaces__srv__SetDisplayClass_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
screen_interfaces__srv__SetDisplayClass_Request__Sequence__init(screen_interfaces__srv__SetDisplayClass_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Request)) {
      return false;
    }
    data = (screen_interfaces__srv__SetDisplayClass_Request *)allocator.zero_allocate(size, sizeof(screen_interfaces__srv__SetDisplayClass_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = screen_interfaces__srv__SetDisplayClass_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        screen_interfaces__srv__SetDisplayClass_Request__fini(&data[i - 1]);
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
screen_interfaces__srv__SetDisplayClass_Request__Sequence__fini(screen_interfaces__srv__SetDisplayClass_Request__Sequence * array)
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
      screen_interfaces__srv__SetDisplayClass_Request__fini(&array->data[i]);
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

screen_interfaces__srv__SetDisplayClass_Request__Sequence *
screen_interfaces__srv__SetDisplayClass_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Request__Sequence * array = (screen_interfaces__srv__SetDisplayClass_Request__Sequence *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = screen_interfaces__srv__SetDisplayClass_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
screen_interfaces__srv__SetDisplayClass_Request__Sequence__destroy(screen_interfaces__srv__SetDisplayClass_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    screen_interfaces__srv__SetDisplayClass_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
screen_interfaces__srv__SetDisplayClass_Request__Sequence__are_equal(const screen_interfaces__srv__SetDisplayClass_Request__Sequence * lhs, const screen_interfaces__srv__SetDisplayClass_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
screen_interfaces__srv__SetDisplayClass_Request__Sequence__copy(
  const screen_interfaces__srv__SetDisplayClass_Request__Sequence * input,
  screen_interfaces__srv__SetDisplayClass_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(screen_interfaces__srv__SetDisplayClass_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    screen_interfaces__srv__SetDisplayClass_Request * data =
      (screen_interfaces__srv__SetDisplayClass_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!screen_interfaces__srv__SetDisplayClass_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          screen_interfaces__srv__SetDisplayClass_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
screen_interfaces__srv__SetDisplayClass_Response__init(screen_interfaces__srv__SetDisplayClass_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    screen_interfaces__srv__SetDisplayClass_Response__fini(msg);
    return false;
  }
  return true;
}

void
screen_interfaces__srv__SetDisplayClass_Response__fini(screen_interfaces__srv__SetDisplayClass_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
screen_interfaces__srv__SetDisplayClass_Response__are_equal(const screen_interfaces__srv__SetDisplayClass_Response * lhs, const screen_interfaces__srv__SetDisplayClass_Response * rhs)
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
screen_interfaces__srv__SetDisplayClass_Response__copy(
  const screen_interfaces__srv__SetDisplayClass_Response * input,
  screen_interfaces__srv__SetDisplayClass_Response * output)
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

screen_interfaces__srv__SetDisplayClass_Response *
screen_interfaces__srv__SetDisplayClass_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Response * msg = (screen_interfaces__srv__SetDisplayClass_Response *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(screen_interfaces__srv__SetDisplayClass_Response));
  bool success = screen_interfaces__srv__SetDisplayClass_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
screen_interfaces__srv__SetDisplayClass_Response__destroy(screen_interfaces__srv__SetDisplayClass_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    screen_interfaces__srv__SetDisplayClass_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
screen_interfaces__srv__SetDisplayClass_Response__Sequence__init(screen_interfaces__srv__SetDisplayClass_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Response)) {
      return false;
    }
    data = (screen_interfaces__srv__SetDisplayClass_Response *)allocator.zero_allocate(size, sizeof(screen_interfaces__srv__SetDisplayClass_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = screen_interfaces__srv__SetDisplayClass_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        screen_interfaces__srv__SetDisplayClass_Response__fini(&data[i - 1]);
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
screen_interfaces__srv__SetDisplayClass_Response__Sequence__fini(screen_interfaces__srv__SetDisplayClass_Response__Sequence * array)
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
      screen_interfaces__srv__SetDisplayClass_Response__fini(&array->data[i]);
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

screen_interfaces__srv__SetDisplayClass_Response__Sequence *
screen_interfaces__srv__SetDisplayClass_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Response__Sequence * array = (screen_interfaces__srv__SetDisplayClass_Response__Sequence *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = screen_interfaces__srv__SetDisplayClass_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
screen_interfaces__srv__SetDisplayClass_Response__Sequence__destroy(screen_interfaces__srv__SetDisplayClass_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    screen_interfaces__srv__SetDisplayClass_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
screen_interfaces__srv__SetDisplayClass_Response__Sequence__are_equal(const screen_interfaces__srv__SetDisplayClass_Response__Sequence * lhs, const screen_interfaces__srv__SetDisplayClass_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
screen_interfaces__srv__SetDisplayClass_Response__Sequence__copy(
  const screen_interfaces__srv__SetDisplayClass_Response__Sequence * input,
  screen_interfaces__srv__SetDisplayClass_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(screen_interfaces__srv__SetDisplayClass_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    screen_interfaces__srv__SetDisplayClass_Response * data =
      (screen_interfaces__srv__SetDisplayClass_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!screen_interfaces__srv__SetDisplayClass_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          screen_interfaces__srv__SetDisplayClass_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__functions.h"

bool
screen_interfaces__srv__SetDisplayClass_Event__init(screen_interfaces__srv__SetDisplayClass_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    screen_interfaces__srv__SetDisplayClass_Event__fini(msg);
    return false;
  }
  // request
  if (!screen_interfaces__srv__SetDisplayClass_Request__Sequence__init(&msg->request, 0)) {
    screen_interfaces__srv__SetDisplayClass_Event__fini(msg);
    return false;
  }
  // response
  if (!screen_interfaces__srv__SetDisplayClass_Response__Sequence__init(&msg->response, 0)) {
    screen_interfaces__srv__SetDisplayClass_Event__fini(msg);
    return false;
  }
  return true;
}

void
screen_interfaces__srv__SetDisplayClass_Event__fini(screen_interfaces__srv__SetDisplayClass_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  screen_interfaces__srv__SetDisplayClass_Request__Sequence__fini(&msg->request);
  // response
  screen_interfaces__srv__SetDisplayClass_Response__Sequence__fini(&msg->response);
}

bool
screen_interfaces__srv__SetDisplayClass_Event__are_equal(const screen_interfaces__srv__SetDisplayClass_Event * lhs, const screen_interfaces__srv__SetDisplayClass_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!screen_interfaces__srv__SetDisplayClass_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!screen_interfaces__srv__SetDisplayClass_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
screen_interfaces__srv__SetDisplayClass_Event__copy(
  const screen_interfaces__srv__SetDisplayClass_Event * input,
  screen_interfaces__srv__SetDisplayClass_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!screen_interfaces__srv__SetDisplayClass_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!screen_interfaces__srv__SetDisplayClass_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

screen_interfaces__srv__SetDisplayClass_Event *
screen_interfaces__srv__SetDisplayClass_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Event * msg = (screen_interfaces__srv__SetDisplayClass_Event *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(screen_interfaces__srv__SetDisplayClass_Event));
  bool success = screen_interfaces__srv__SetDisplayClass_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
screen_interfaces__srv__SetDisplayClass_Event__destroy(screen_interfaces__srv__SetDisplayClass_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    screen_interfaces__srv__SetDisplayClass_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
screen_interfaces__srv__SetDisplayClass_Event__Sequence__init(screen_interfaces__srv__SetDisplayClass_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Event)) {
      return false;
    }
    data = (screen_interfaces__srv__SetDisplayClass_Event *)allocator.zero_allocate(size, sizeof(screen_interfaces__srv__SetDisplayClass_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = screen_interfaces__srv__SetDisplayClass_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        screen_interfaces__srv__SetDisplayClass_Event__fini(&data[i - 1]);
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
screen_interfaces__srv__SetDisplayClass_Event__Sequence__fini(screen_interfaces__srv__SetDisplayClass_Event__Sequence * array)
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
      screen_interfaces__srv__SetDisplayClass_Event__fini(&array->data[i]);
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

screen_interfaces__srv__SetDisplayClass_Event__Sequence *
screen_interfaces__srv__SetDisplayClass_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  screen_interfaces__srv__SetDisplayClass_Event__Sequence * array = (screen_interfaces__srv__SetDisplayClass_Event__Sequence *)allocator.allocate(sizeof(screen_interfaces__srv__SetDisplayClass_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = screen_interfaces__srv__SetDisplayClass_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
screen_interfaces__srv__SetDisplayClass_Event__Sequence__destroy(screen_interfaces__srv__SetDisplayClass_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    screen_interfaces__srv__SetDisplayClass_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
screen_interfaces__srv__SetDisplayClass_Event__Sequence__are_equal(const screen_interfaces__srv__SetDisplayClass_Event__Sequence * lhs, const screen_interfaces__srv__SetDisplayClass_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
screen_interfaces__srv__SetDisplayClass_Event__Sequence__copy(
  const screen_interfaces__srv__SetDisplayClass_Event__Sequence * input,
  screen_interfaces__srv__SetDisplayClass_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(screen_interfaces__srv__SetDisplayClass_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(screen_interfaces__srv__SetDisplayClass_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    screen_interfaces__srv__SetDisplayClass_Event * data =
      (screen_interfaces__srv__SetDisplayClass_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!screen_interfaces__srv__SetDisplayClass_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          screen_interfaces__srv__SetDisplayClass_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!screen_interfaces__srv__SetDisplayClass_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
