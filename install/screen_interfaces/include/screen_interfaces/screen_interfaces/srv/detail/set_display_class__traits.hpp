// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from screen_interfaces:srv/SetDisplayClass.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "screen_interfaces/srv/set_display_class.hpp"


#ifndef SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__TRAITS_HPP_
#define SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace screen_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetDisplayClass_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: category
  {
    out << "category: ";
    rosidl_generator_traits::value_to_yaml(msg.category, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetDisplayClass_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: category
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "category: ";
    rosidl_generator_traits::value_to_yaml(msg.category, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetDisplayClass_Request & msg, bool use_flow_style = false)
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

}  // namespace screen_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use screen_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const screen_interfaces::srv::SetDisplayClass_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  screen_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use screen_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const screen_interfaces::srv::SetDisplayClass_Request & msg)
{
  return screen_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<screen_interfaces::srv::SetDisplayClass_Request>()
{
  return "screen_interfaces::srv::SetDisplayClass_Request";
}

template<>
inline const char * name<screen_interfaces::srv::SetDisplayClass_Request>()
{
  return "screen_interfaces/srv/SetDisplayClass_Request";
}

template<>
struct has_fixed_size<screen_interfaces::srv::SetDisplayClass_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<screen_interfaces::srv::SetDisplayClass_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<screen_interfaces::srv::SetDisplayClass_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace screen_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetDisplayClass_Response & msg,
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
  const SetDisplayClass_Response & msg,
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

inline std::string to_yaml(const SetDisplayClass_Response & msg, bool use_flow_style = false)
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

}  // namespace screen_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use screen_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const screen_interfaces::srv::SetDisplayClass_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  screen_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use screen_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const screen_interfaces::srv::SetDisplayClass_Response & msg)
{
  return screen_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<screen_interfaces::srv::SetDisplayClass_Response>()
{
  return "screen_interfaces::srv::SetDisplayClass_Response";
}

template<>
inline const char * name<screen_interfaces::srv::SetDisplayClass_Response>()
{
  return "screen_interfaces/srv/SetDisplayClass_Response";
}

template<>
struct has_fixed_size<screen_interfaces::srv::SetDisplayClass_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<screen_interfaces::srv::SetDisplayClass_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<screen_interfaces::srv::SetDisplayClass_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace screen_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetDisplayClass_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetDisplayClass_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetDisplayClass_Event & msg, bool use_flow_style = false)
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

}  // namespace screen_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use screen_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const screen_interfaces::srv::SetDisplayClass_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  screen_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use screen_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const screen_interfaces::srv::SetDisplayClass_Event & msg)
{
  return screen_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<screen_interfaces::srv::SetDisplayClass_Event>()
{
  return "screen_interfaces::srv::SetDisplayClass_Event";
}

template<>
inline const char * name<screen_interfaces::srv::SetDisplayClass_Event>()
{
  return "screen_interfaces/srv/SetDisplayClass_Event";
}

template<>
struct has_fixed_size<screen_interfaces::srv::SetDisplayClass_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<screen_interfaces::srv::SetDisplayClass_Event>
  : std::integral_constant<bool, has_bounded_size<screen_interfaces::srv::SetDisplayClass_Request>::value && has_bounded_size<screen_interfaces::srv::SetDisplayClass_Response>::value && has_bounded_size<service_msgs::msg::ServiceEventInfo>::value> {};

template<>
struct is_message<screen_interfaces::srv::SetDisplayClass_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<screen_interfaces::srv::SetDisplayClass>()
{
  return "screen_interfaces::srv::SetDisplayClass";
}

template<>
inline const char * name<screen_interfaces::srv::SetDisplayClass>()
{
  return "screen_interfaces/srv/SetDisplayClass";
}

template<>
struct has_fixed_size<screen_interfaces::srv::SetDisplayClass>
  : std::integral_constant<
    bool,
    has_fixed_size<screen_interfaces::srv::SetDisplayClass_Request>::value &&
    has_fixed_size<screen_interfaces::srv::SetDisplayClass_Response>::value
  >
{
};

template<>
struct has_bounded_size<screen_interfaces::srv::SetDisplayClass>
  : std::integral_constant<
    bool,
    has_bounded_size<screen_interfaces::srv::SetDisplayClass_Request>::value &&
    has_bounded_size<screen_interfaces::srv::SetDisplayClass_Response>::value
  >
{
};

template<>
struct is_service<screen_interfaces::srv::SetDisplayClass>
  : std::true_type
{
};

template<>
struct is_service_request<screen_interfaces::srv::SetDisplayClass_Request>
  : std::true_type
{
};

template<>
struct is_service_response<screen_interfaces::srv::SetDisplayClass_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__TRAITS_HPP_
