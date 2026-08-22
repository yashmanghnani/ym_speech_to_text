# Changelog

## 0.0.3

### Added

* Added Linux plugin registration and build support across the Flutter plugin
  platforms.
* Added safe Linux fallback behavior when a native speech backend is not
  available.
* Added current Flutter/Dart compatibility metadata and refreshed package
  dependencies.

### Changed

* Updated Android Gradle Plugin, Gradle wrapper, Kotlin, compile SDK, target SDK,
  and Android lifecycle handling.
* Updated Android microphone and Bluetooth permission declarations and runtime
  permission flow.
* Updated iOS/macOS deployment targets and shared Darwin plugin compatibility.
* Improved web Speech Recognition startup, locale fallback, and final-result
  handling.
* Kept Windows support connected through `speech_to_text_windows`.
* Removed generated and local tooling files from the published package inputs.

### Compatibility

* Flutter `>=3.29.3`
* Dart `>=3.4.0 <4.0.0`
* Android minimum SDK 21
* iOS 12.0+
* macOS 10.14+

## 0.0.2

* Initial release.
