//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <ym_speech_to_text/speech_to_text_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) ym_speech_to_text_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "SpeechToTextPlugin");
  speech_to_text_plugin_register_with_registrar(ym_speech_to_text_registrar);
}
