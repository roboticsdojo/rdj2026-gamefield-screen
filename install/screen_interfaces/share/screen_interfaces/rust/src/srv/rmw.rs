#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



#[link(name = "screen_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__screen_interfaces__srv__SetDisplayClass_Request() -> *const std::ffi::c_void;
}

#[link(name = "screen_interfaces__rosidl_generator_c")]
extern "C" {
    fn screen_interfaces__srv__SetDisplayClass_Request__init(msg: *mut SetDisplayClass_Request) -> bool;
    fn screen_interfaces__srv__SetDisplayClass_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Request>, size: usize) -> bool;
    fn screen_interfaces__srv__SetDisplayClass_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Request>);
    fn screen_interfaces__srv__SetDisplayClass_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SetDisplayClass_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Request>) -> bool;
}

// Corresponds to screen_interfaces__srv__SetDisplayClass_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetDisplayClass_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub category: rosidl_runtime_rs::String,

}



impl Default for SetDisplayClass_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !screen_interfaces__srv__SetDisplayClass_Request__init(&mut msg as *mut _) {
        panic!("Call to screen_interfaces__srv__SetDisplayClass_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SetDisplayClass_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SetDisplayClass_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SetDisplayClass_Request where Self: Sized {
  const TYPE_NAME: &'static str = "screen_interfaces/srv/SetDisplayClass_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__screen_interfaces__srv__SetDisplayClass_Request() }
  }
}


#[link(name = "screen_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__screen_interfaces__srv__SetDisplayClass_Response() -> *const std::ffi::c_void;
}

#[link(name = "screen_interfaces__rosidl_generator_c")]
extern "C" {
    fn screen_interfaces__srv__SetDisplayClass_Response__init(msg: *mut SetDisplayClass_Response) -> bool;
    fn screen_interfaces__srv__SetDisplayClass_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Response>, size: usize) -> bool;
    fn screen_interfaces__srv__SetDisplayClass_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Response>);
    fn screen_interfaces__srv__SetDisplayClass_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SetDisplayClass_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<SetDisplayClass_Response>) -> bool;
}

// Corresponds to screen_interfaces__srv__SetDisplayClass_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetDisplayClass_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: rosidl_runtime_rs::String,

}



impl Default for SetDisplayClass_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !screen_interfaces__srv__SetDisplayClass_Response__init(&mut msg as *mut _) {
        panic!("Call to screen_interfaces__srv__SetDisplayClass_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SetDisplayClass_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { screen_interfaces__srv__SetDisplayClass_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SetDisplayClass_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SetDisplayClass_Response where Self: Sized {
  const TYPE_NAME: &'static str = "screen_interfaces/srv/SetDisplayClass_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__screen_interfaces__srv__SetDisplayClass_Response() }
  }
}






#[link(name = "screen_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__screen_interfaces__srv__SetDisplayClass() -> *const std::ffi::c_void;
}

// Corresponds to screen_interfaces__srv__SetDisplayClass
#[allow(missing_docs, non_camel_case_types)]
pub struct SetDisplayClass;

impl rosidl_runtime_rs::Service for SetDisplayClass {
    type Request = SetDisplayClass_Request;
    type Response = SetDisplayClass_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__screen_interfaces__srv__SetDisplayClass() }
    }
}


