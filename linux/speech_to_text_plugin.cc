#include "include/ym_speech_to_text/speech_to_text_plugin.h"

#include <flutter_linux/flutter_linux.h>

#include <cstring>

namespace {

constexpr char kChannelName[] = "plugin.csdcorp.com/speech_to_text";

FlMethodResponse* UnsupportedResponse() {
  // Returning false keeps the public Dart API consistent with platforms where
  // the operating system has no speech recognizer available. It also avoids
  // turning an expected capability check into an unhandled PlatformException.
  g_autoptr(FlValue) value = fl_value_new_bool(FALSE);
  return FL_METHOD_RESPONSE(fl_method_success_response_new(value));
}

void HandleMethodCall(FlMethodChannel* channel, FlMethodCall* method_call,
                      gpointer user_data) {
  (void)channel;
  (void)user_data;
  const gchar* method = fl_method_call_get_name(method_call);
  g_autoptr(FlMethodResponse) response = nullptr;

  if (strcmp(method, "has_permission") == 0) {
    g_autoptr(FlValue) value = fl_value_new_bool(FALSE);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(value));
  } else if (strcmp(method, "initialize") == 0 ||
             strcmp(method, "listen") == 0) {
    response = UnsupportedResponse();
  } else if (strcmp(method, "stop") == 0 ||
             strcmp(method, "cancel") == 0) {
    g_autoptr(FlValue) value = fl_value_new_bool(TRUE);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(value));
  } else if (strcmp(method, "locales") == 0) {
    g_autoptr(FlValue) value = fl_value_new_list();
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(value));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

}  // namespace

void speech_to_text_plugin_register_with_registrar(
    FlPluginRegistrar* registrar) {
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel = fl_method_channel_new(
      fl_plugin_registrar_get_messenger(registrar), kChannelName,
      FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, HandleMethodCall, nullptr,
                                            nullptr);
}
