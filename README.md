# ym_speech_to_text

[![pub package](https://img.shields.io/badge/pub-v0.0.3-blue)](https://pub.dev/packages/ym_speech_to_text)

`ym_speech_to_text` is a Flutter plugin for short, intermittent speech-to-text recognition using the native or browser speech service available on the target platform.

It supports Android, iOS, macOS, web, Windows, and Linux builds. Linux is currently build-compatible, but this package does not bundle a Linux speech-recognition backend, so speech initialization returns `false` there.

## Platform support

| Capability | Android | iOS | macOS | Web | Windows | Linux |
| --- | :---: | :---: | :---: | :---: | :---: | :---: |
| Build | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| Speech recognition | ✅ | ✅ | ✅ | ✅* | ✅ | — |

\* Web support depends on the browser's Web Speech API implementation and user permission. Browser behavior, available locales, and on-device support can vary.

## Requirements

- Flutter `>=3.29.3`
- Dart `>=3.4.0 <4.0.0`
- Android compile/target SDK 36, minimum SDK 21
- iOS 12.0 or newer
- macOS 10.14 or newer
- Windows recognition is provided through [`speech_to_text_windows`](https://pub.dev/packages/speech_to_text_windows)
- Linux currently provides build and channel registration support only

## Installation

Add the package to `pubspec.yaml`:

```yaml
dependencies:
  ym_speech_to_text: ^0.0.3
```

Then run:

```bash
flutter pub get
```

## Basic usage

Initialize the recognizer once, then start listening only after initialization succeeds:

```dart
import 'package:flutter/foundation.dart';
import 'package:ym_speech_to_text/speech_to_text.dart';

final speech = SpeechToText();

Future<void> startSpeech() async {
  final available = await speech.initialize(
    onStatus: (status) => debugPrint('status: $status'),
    onError: (error) => debugPrint(
      'error: ${error.errorMsg}, permanent: ${error.permanent}',
    ),
  );

  if (!available) {
    debugPrint('Speech recognition is unavailable');
    return;
  }

  await speech.listen(
    onResult: (result) {
      debugPrint('text: ${result.recognizedWords}');
      debugPrint('final: ${result.finalResult}');
    },
    listenOptions: SpeechListenOptions(
      partialResults: true,
      cancelOnError: true,
    ),
  );
}

Future<void> stopSpeech() => speech.stop();
Future<void> cancelSpeech() => speech.cancel();
```

`SpeechToText()` is a singleton. Call `initialize()` once for the app/session and reuse that instance. A successful `initialize()` call is required before `listen()`, `stop()`, or `cancel()`.

## Listening options

`SpeechListenOptions` is the preferred API for configuring a session:

| Option | Purpose |
| --- | --- |
| `partialResults` | Report interim text while the user is speaking. Set `false` for final results only. |
| `cancelOnError` | Cancel the session when a permanent error is received. |
| `onDevice` | Request on-device recognition. The session fails if it is unavailable. |
| `listenMode` | iOS recognition intent: `deviceDefault`, `dictation`, `search`, or `confirmation`. |
| `sampleRate` | Optional iOS sample rate; `44100` may help on devices with sample-rate issues. |
| `autoPunctuation` | Request automatic punctuation where supported, currently iOS. |
| `enableHapticFeedback` | Enable haptics where supported, currently iOS. |
| `pauseFor` | Stop after the configured silence duration. |
| `listenFor` | Limit the maximum listening duration. |
| `localeId` | Request a locale where supported by the platform. |

The older top-level `listen` parameters such as `partialResults`, `onDevice`, `listenMode`, and `sampleRate` remain available for compatibility but are deprecated. Prefer `SpeechListenOptions` in new code.

The operating system or browser can impose a shorter recognition timeout than `listenFor`. Calling `stop()` ends the session and allows a final result; calling `cancel()` ends it without a final result.

## Initialization options

Platform-specific behavior can be adjusted with `SpeechConfigOption`:

```dart
final available = await speech.initialize(
  options: [
    SpeechToText.androidAlwaysUseStop,
    SpeechToText.webDoNotAggregate,
  ],
);
```

Available options are:

- `SpeechToText.androidAlwaysUseStop`
- `SpeechToText.androidIntentLookup`
- `SpeechToText.androidNoBluetooth`
- `SpeechToText.iosNoBluetooth`
- `SpeechToText.webDoNotAggregate`

The default final-result timeout is two seconds and can be changed with the `finalTimeout` argument to `initialize()`.

## Results and state

Each `SpeechRecognitionResult` provides:

- `recognizedWords`: best transcription
- `alternates`: alternate transcriptions
- `finalResult`: whether the result is final
- `confidence`: confidence score when supplied by the platform

Useful `SpeechToText` state and helpers include `isListening`, `isNotListening`, `isAvailable`, `hasPermission`, `lastError`, `lastSoundLevel`, `locales()`, and `systemLocale()`.

For applications that prefer a notifier and event stream, use the optional `SpeechToTextProvider`:

```dart
import 'package:flutter/foundation.dart';
import 'package:ym_speech_to_text/speech_to_text.dart';
import 'package:ym_speech_to_text/speech_to_text_provider.dart';

final provider = SpeechToTextProvider(SpeechToText());

final available = await provider.initialize();
if (available) {
  provider.stream.listen((event) {
    final result = event.recognitionResult;
    if (result != null) {
      debugPrint(result.recognizedWords);
    }
  });
}
```

The provider uses Flutter's `ChangeNotifier`; the package does not require the separate `provider` package.

## Permissions and platform notes

### Android

The plugin declares the microphone and Bluetooth permissions it needs. If the host app performs speech-service discovery or uses network recognition, keep these in the app manifest as appropriate:

```xml
<uses-permission android:name="android.permission.RECORD_AUDIO" />
<uses-permission android:name="android.permission.INTERNET" />

<queries>
    <intent>
        <action android:name="android.speech.RecognitionService" />
    </intent>
</queries>
```

Requesting microphone permission is handled by the plugin during initialization. The device must also have a compatible speech recognition service and language installed.

### iOS and macOS

Add the following usage descriptions to the host application's `Info.plist`:

```xml
<key>NSMicrophoneUsageDescription</key>
<string>This app uses the microphone for speech recognition.</string>
<key>NSSpeechRecognitionUsageDescription</key>
<string>This app converts speech to text.</string>
```

For macOS, the app also needs the audio-input entitlement:

```xml
<key>com.apple.security.device.audio-input</key>
<true/>
```

### Web

Run the app in a browser with Web Speech API support, allow microphone/speech permission, and test the exact browser versions required by your product. The plugin uses the standard `SpeechRecognition` API and the `webkitSpeechRecognition` fallback where available.

### Windows

Windows support is registered through the [`speech_to_text_windows`](https://pub.dev/packages/speech_to_text_windows) implementation included by this package.

### Linux

Linux builds and plugin registration are supported. No Linux speech backend is bundled yet, so `initialize()`, `hasPermission`, and `listen()` report unavailable behavior until a native backend is added.

## Example app

The repository includes a multi-platform example:

```bash
cd example
flutter pub get
flutter run
```

It also contains basic, provider, and stress examples in `example/lib`.

## Development and validation

From the package root:

```bash
flutter pub get
dart format lib test example/lib
flutter analyze
flutter test
flutter pub publish --dry-run
```

To validate the main build targets:

```bash
cd example
flutter build web --release
flutter build apk --debug
```

## Troubleshooting

- If Android initialization returns `false`, check microphone permission, the installed speech service, device language settings, and the Android recognizer query.
- iOS and macOS require both usage descriptions and an actual speech-capable device or correctly configured simulator.
- Web recognition depends on browser support and permission; inspect browser console errors when `SpeechRecognition` is unavailable.
- Use `stop()` or `cancel()` when a screen/widget is disposed so the native recognition session is released.
- Linux is currently build-only for speech recognition.

## Changelog and license

See [CHANGELOG.md](CHANGELOG.md) for release notes. This project is distributed under the [MIT License](LICENSE).
