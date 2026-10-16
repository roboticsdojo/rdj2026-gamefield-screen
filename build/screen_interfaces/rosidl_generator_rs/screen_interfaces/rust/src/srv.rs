#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};




// Corresponds to screen_interfaces__srv__SetDisplayClass_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetDisplayClass_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub category: std::string::String,

}



impl Default for SetDisplayClass_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::SetDisplayClass_Request::default())
  }
}

impl rosidl_runtime_rs::Message for SetDisplayClass_Request {
  type RmwMsg = super::srv::rmw::SetDisplayClass_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        category: msg.category.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        category: msg.category.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      category: msg.category.to_string(),
    }
  }
}


// Corresponds to screen_interfaces__srv__SetDisplayClass_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SetDisplayClass_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: std::string::String,

}



impl Default for SetDisplayClass_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::SetDisplayClass_Response::default())
  }
}

impl rosidl_runtime_rs::Message for SetDisplayClass_Response {
  type RmwMsg = super::srv::rmw::SetDisplayClass_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        success: msg.success,
        message: msg.message.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      success: msg.success,
        message: msg.message.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      success: msg.success,
      message: msg.message.to_string(),
    }
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


