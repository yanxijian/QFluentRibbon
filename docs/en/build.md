# Build

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

Qt 6.8+ Widgets (+ Test); **installed** QThemeEngine on `CMAKE_PREFIX_PATH` by default.

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;D:\Codes\prefix ^
  -DCMAKE_INSTALL_PREFIX=D:\Codes\prefix ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON
cmake --build build
cmake --install build
build\qfr_gallery.exe
```

Dev-only sibling embed: `-DQFR_DEV_EMBED_QTE=ON -DQFR_INSTALL=OFF` (incompatible with install).

Consumers: `find_package(QFluentRibbon)` → `QFluentRibbon::qfluentribbon` (also pulls QThemeEngine). Runtime needs `qfluentribbon.dll` + `qtheme_engine.dll` beside the exe.

See also: [hci-dpi-checklist.md](hci-dpi-checklist.md) · [mps-integration.md](mps-integration.md).
