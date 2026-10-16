# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target screen_interfaces::screen_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${screen_interfaces_TARGETS}.
if(screen_interfaces_TARGETS AND NOT TARGET screen_interfaces::screen_interfaces)
  add_library(screen_interfaces::screen_interfaces INTERFACE IMPORTED)
  set_target_properties(screen_interfaces::screen_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${screen_interfaces_TARGETS}")
endif()
