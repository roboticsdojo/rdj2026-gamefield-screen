// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from screen_interfaces:srv/SetDisplayClass.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "screen_interfaces/srv/set_display_class.hpp"


#ifndef SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__BUILDER_HPP_
#define SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "screen_interfaces/srv/detail/set_display_class__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace screen_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetDisplayClass_Request_category
{
public:
  Init_SetDisplayClass_Request_category()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::screen_interfaces::srv::SetDisplayClass_Request category(::screen_interfaces::srv::SetDisplayClass_Request::_category_type arg)
  {
    msg_.category = std::move(arg);
    return std::move(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::screen_interfaces::srv::SetDisplayClass_Request>()
{
  return screen_interfaces::srv::builder::Init_SetDisplayClass_Request_category();
}

}  // namespace screen_interfaces


namespace screen_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetDisplayClass_Response_message
{
public:
  explicit Init_SetDisplayClass_Response_message(::screen_interfaces::srv::SetDisplayClass_Response & msg)
  : msg_(msg)
  {}
  ::screen_interfaces::srv::SetDisplayClass_Response message(::screen_interfaces::srv::SetDisplayClass_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Response msg_;
};

class Init_SetDisplayClass_Response_success
{
public:
  Init_SetDisplayClass_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetDisplayClass_Response_message success(::screen_interfaces::srv::SetDisplayClass_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_SetDisplayClass_Response_message(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::screen_interfaces::srv::SetDisplayClass_Response>()
{
  return screen_interfaces::srv::builder::Init_SetDisplayClass_Response_success();
}

}  // namespace screen_interfaces


namespace screen_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetDisplayClass_Event_response
{
public:
  explicit Init_SetDisplayClass_Event_response(::screen_interfaces::srv::SetDisplayClass_Event & msg)
  : msg_(msg)
  {}
  ::screen_interfaces::srv::SetDisplayClass_Event response(::screen_interfaces::srv::SetDisplayClass_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Event msg_;
};

class Init_SetDisplayClass_Event_request
{
public:
  explicit Init_SetDisplayClass_Event_request(::screen_interfaces::srv::SetDisplayClass_Event & msg)
  : msg_(msg)
  {}
  Init_SetDisplayClass_Event_response request(::screen_interfaces::srv::SetDisplayClass_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_SetDisplayClass_Event_response(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Event msg_;
};

class Init_SetDisplayClass_Event_info
{
public:
  Init_SetDisplayClass_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetDisplayClass_Event_request info(::screen_interfaces::srv::SetDisplayClass_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_SetDisplayClass_Event_request(msg_);
  }

private:
  ::screen_interfaces::srv::SetDisplayClass_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::screen_interfaces::srv::SetDisplayClass_Event>()
{
  return screen_interfaces::srv::builder::Init_SetDisplayClass_Event_info();
}

}  // namespace screen_interfaces

#endif  // SCREEN_INTERFACES__SRV__DETAIL__SET_DISPLAY_CLASS__BUILDER_HPP_
