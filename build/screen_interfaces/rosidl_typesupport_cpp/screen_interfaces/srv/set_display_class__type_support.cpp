// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from screen_interfaces:srv/SetDisplayClass.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "screen_interfaces/srv/detail/set_display_class__functions.h"
#include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace screen_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetDisplayClass_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetDisplayClass_Request_type_support_ids_t;

static const _SetDisplayClass_Request_type_support_ids_t _SetDisplayClass_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetDisplayClass_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetDisplayClass_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetDisplayClass_Request_type_support_symbol_names_t _SetDisplayClass_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, screen_interfaces, srv, SetDisplayClass_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, screen_interfaces, srv, SetDisplayClass_Request)),
  }
};

typedef struct _SetDisplayClass_Request_type_support_data_t
{
  void * data[2];
} _SetDisplayClass_Request_type_support_data_t;

static _SetDisplayClass_Request_type_support_data_t _SetDisplayClass_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetDisplayClass_Request_message_typesupport_map = {
  2,
  "screen_interfaces",
  &_SetDisplayClass_Request_message_typesupport_ids.typesupport_identifier[0],
  &_SetDisplayClass_Request_message_typesupport_symbol_names.symbol_name[0],
  &_SetDisplayClass_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SetDisplayClass_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetDisplayClass_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &screen_interfaces__srv__SetDisplayClass_Request__get_type_hash,
  &screen_interfaces__srv__SetDisplayClass_Request__get_type_description,
  &screen_interfaces__srv__SetDisplayClass_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace screen_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Request>()
{
  return &::screen_interfaces::srv::rosidl_typesupport_cpp::SetDisplayClass_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, screen_interfaces, srv, SetDisplayClass_Request)() {
  return get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__functions.h"
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace screen_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetDisplayClass_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetDisplayClass_Response_type_support_ids_t;

static const _SetDisplayClass_Response_type_support_ids_t _SetDisplayClass_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetDisplayClass_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetDisplayClass_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetDisplayClass_Response_type_support_symbol_names_t _SetDisplayClass_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, screen_interfaces, srv, SetDisplayClass_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, screen_interfaces, srv, SetDisplayClass_Response)),
  }
};

typedef struct _SetDisplayClass_Response_type_support_data_t
{
  void * data[2];
} _SetDisplayClass_Response_type_support_data_t;

static _SetDisplayClass_Response_type_support_data_t _SetDisplayClass_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetDisplayClass_Response_message_typesupport_map = {
  2,
  "screen_interfaces",
  &_SetDisplayClass_Response_message_typesupport_ids.typesupport_identifier[0],
  &_SetDisplayClass_Response_message_typesupport_symbol_names.symbol_name[0],
  &_SetDisplayClass_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SetDisplayClass_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetDisplayClass_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &screen_interfaces__srv__SetDisplayClass_Response__get_type_hash,
  &screen_interfaces__srv__SetDisplayClass_Response__get_type_description,
  &screen_interfaces__srv__SetDisplayClass_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace screen_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Response>()
{
  return &::screen_interfaces::srv::rosidl_typesupport_cpp::SetDisplayClass_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, screen_interfaces, srv, SetDisplayClass_Response)() {
  return get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__functions.h"
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace screen_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetDisplayClass_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetDisplayClass_Event_type_support_ids_t;

static const _SetDisplayClass_Event_type_support_ids_t _SetDisplayClass_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetDisplayClass_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetDisplayClass_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetDisplayClass_Event_type_support_symbol_names_t _SetDisplayClass_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, screen_interfaces, srv, SetDisplayClass_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, screen_interfaces, srv, SetDisplayClass_Event)),
  }
};

typedef struct _SetDisplayClass_Event_type_support_data_t
{
  void * data[2];
} _SetDisplayClass_Event_type_support_data_t;

static _SetDisplayClass_Event_type_support_data_t _SetDisplayClass_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetDisplayClass_Event_message_typesupport_map = {
  2,
  "screen_interfaces",
  &_SetDisplayClass_Event_message_typesupport_ids.typesupport_identifier[0],
  &_SetDisplayClass_Event_message_typesupport_symbol_names.symbol_name[0],
  &_SetDisplayClass_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t SetDisplayClass_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetDisplayClass_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &screen_interfaces__srv__SetDisplayClass_Event__get_type_hash,
  &screen_interfaces__srv__SetDisplayClass_Event__get_type_description,
  &screen_interfaces__srv__SetDisplayClass_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace screen_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Event>()
{
  return &::screen_interfaces::srv::rosidl_typesupport_cpp::SetDisplayClass_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, screen_interfaces, srv, SetDisplayClass_Event)() {
  return get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace screen_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _SetDisplayClass_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _SetDisplayClass_type_support_ids_t;

static const _SetDisplayClass_type_support_ids_t _SetDisplayClass_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _SetDisplayClass_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _SetDisplayClass_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _SetDisplayClass_type_support_symbol_names_t _SetDisplayClass_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, screen_interfaces, srv, SetDisplayClass)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, screen_interfaces, srv, SetDisplayClass)),
  }
};

typedef struct _SetDisplayClass_type_support_data_t
{
  void * data[2];
} _SetDisplayClass_type_support_data_t;

static _SetDisplayClass_type_support_data_t _SetDisplayClass_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _SetDisplayClass_service_typesupport_map = {
  2,
  "screen_interfaces",
  &_SetDisplayClass_service_typesupport_ids.typesupport_identifier[0],
  &_SetDisplayClass_service_typesupport_symbol_names.symbol_name[0],
  &_SetDisplayClass_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t SetDisplayClass_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_SetDisplayClass_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<screen_interfaces::srv::SetDisplayClass_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<screen_interfaces::srv::SetDisplayClass>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<screen_interfaces::srv::SetDisplayClass>,
  &screen_interfaces__srv__SetDisplayClass__get_type_hash,
  &screen_interfaces__srv__SetDisplayClass__get_type_description,
  &screen_interfaces__srv__SetDisplayClass__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace screen_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<screen_interfaces::srv::SetDisplayClass>()
{
  return &::screen_interfaces::srv::rosidl_typesupport_cpp::SetDisplayClass_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, screen_interfaces, srv, SetDisplayClass)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<screen_interfaces::srv::SetDisplayClass>();
}

#ifdef __cplusplus
}
#endif
