# ZipAlign for Android

Android port of [`zipalign` tool](http://developer.android.com/tools/help/zipalign.html).

[![](https://jitpack.io/v/MuntashirAkon/zipalign-android.svg)](https://jitpack.io/#MuntashirAkon/zipalign-android)

## Quick start
ZipAlign for Android is available via JitPack.

```groovy
// Top level build file
repositories {
    maven { url "https://jitpack.io" }
}

// Add to dependencies section
dependencies {
    implementation 'com.github.MuntashirAkon:zipalign-android:1.0'
}
```

After importing the library, use the `ZipAlign` class to align or verify alignment of a ZIP or APK file.

This library is a native library, so make sure to define architectures/instruction sets based on your need.

## Credits
ZipAlign for Android is based on [android-lib-zipalign](https://github.com/animehunter/android-lib-zipalign) by @animehunter.
