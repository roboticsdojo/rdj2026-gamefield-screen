// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from screen_interfaces:srv/SetDisplayClass.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "screen_interfaces/srv/set_display_class.h"


#ifndef SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__STRUCT_H_
#define SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'category'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetDisplayClass in the package screen_interfaces.
typedef struct screen_interfaces__srv__SetDisplayClass_Request
{
  rosidl_runtime_c__String category;
} screen_interfaces__srv__SetDisplayClass_Request;

// Struct for a sequence of screen_interfaces__srv__SetDisplayClass_Request.
typedef struct screen_interfaces__srv__SetDisplayClass_Request__Sequence
{
  screen_interfaces__srv__SetDisplayClass_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} screen_interfaces__srv__SetDisplayClass_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetDisplayClass in the package screen_interfaces.
typedef struct screen_interfaces__srv__SetDisplayClass_Response
{
  bool success;
  rosidl_runtime_c__String message;
} screen_interfaces__srv__SetDisplayClass_Response;

// Struct for a sequence of screen_interfaces__srv__SetDisplayClass_Response.
typedef struct screen_interfaces__srv__SetDisplayClass_Response__Sequence
{
  screen_interfaces__srv__SetDisplayClass_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} screen_interfaces__srv__SetDisplayClass_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  screen_interfaces__srv__SetDisplayClass_Event__request__MAX_SIZE = 1
};
// response
enum
{
  screen_interfaces__srv__SetDisplayClass_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/SetDisplayClass in the package screen_interfaces.
typedef struct screen_interfaces__srv__SetDisplayClass_Event
{
  service_msgs__msg__ServiceEventInfo info;
  screen_interfaces__srv__SetDisplayClass_Request__Sequence request;
  screen_interfaces__srv__SetDisplayClass_Response__Sequence response;
} screen_interfaces__srv__SetDisplayClass_Event;

// Struct for a sequence of screen_interfaces__srv__SetDisplayClass_Event.
typedef struct screen_interfaces__srv__SetDisplayClass_Event__Sequence
{
  screen_interfaces__srv__SetDisplayClass_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} screen_interfaces__srv__SetDisplayClass_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__STRUCT_H_
