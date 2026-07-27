# Build

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

| Item | Notes |
|------|--------|
| Qt | 6.8+ Widgets (+ Test for unit tests) |
| QThemeEngine | Required only when building the demo (`QFR_BUILD_EXAMPLES`); the `qfr_ribbon` library does **not** link QTE |
| Toolchain | CMake 3.21+, Ninja; MSVC x64 on Windows |

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;D:\Codes\prefix ^
  -DCMAKE_INSTALL_PREFIX=D:\Codes\prefix ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON -DQFR_BUILD_EXAMPLES=ON -DQFR_BUILD_TESTS=ON
cmake --build build
cmake --install build
build\qfr_demo.exe
```

| Artifact | Name |
|----------|------|
| Shared library | `qfr_ribbon` |
| Demo | `qfr_demo` |
| Tests | `qfr_tests`, `qfr_qat_tests` |
| Imported target | `QFluentRibbon::ribbon` |

Consumers: `find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`. Runtime for the demo also needs `qte_engine` beside the exe.

See also: [hci-dpi-checklist.md](hci-dpi-checklist.md).
