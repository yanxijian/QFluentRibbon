# Build (M6)

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

Qt 6.8+ Widgets (+ Test); QThemeEngine via sibling tree, `-DQFR_QTE_SOURCE_DIR=`, or installed package.

```bat
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR% -DQFR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
build\qfr_gallery.exe
```

Install (after QTE is installed): `-DQFR_INSTALL=ON` with `CMAKE_PREFIX_PATH` including QTE (not in-tree `QFR_QTE_SOURCE_DIR`). Consumers: `find_package(QFluentRibbon)` → `QFluentRibbon::qfluentribbon`.

Sibling day-to-day builds keep `QFR_INSTALL=OFF`.

See also: [hci-dpi-checklist.md](hci-dpi-checklist.md) · [mps-integration.md](mps-integration.md).
