# Build (M2)

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

Qt 6.8+ Widgets (+ Test for unit tests); QThemeEngine via sibling `../QThemeEngine`, `-DQFR_QTE_SOURCE_DIR=`, or installed `find_package`.

```bat
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR% -DQFR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
build\qfr_gallery.exe
```

M2: Simplified toggle, group launchers, ScreenTip on hover; Skin combo still follows QTE (no Ribbon QSS).
