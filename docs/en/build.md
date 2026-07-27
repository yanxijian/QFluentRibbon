# Build

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

| Item | Notes |
|------|--------|
| Qt | 6.8+ Widgets (+ Test for unit tests) |
| QThemeEngine | Required only when building the demo (`QFR_BUILD_EXAMPLES`); the `qfr_ribbon` library does **not** link QTE |
| Toolchain | CMake 3.21+, Ninja; MSVC x64 on Windows |

Local shared-library convention uses **`build-shared`**. Point `CMAKE_PREFIX_PATH` at an installed QTE (`qte_engine`).

```bat
:: PREFIX = install root (often sibling prefix/ of the three repos); QTE must already be installed
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DCMAKE_INSTALL_PREFIX=%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON -DQFR_BUILD_EXAMPLES=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
cmake --install build-shared
build-shared\qfr_demo.exe
```

| Artifact | Name |
|----------|------|
| Shared library | `qfr_ribbon` |
| Demo | `qfr_demo` |
| Tests | `qfr_tests`, `qfr_qat_tests` |
| Imported target | `QFluentRibbon::ribbon` |

Consumers: `find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`. Runtime for the demo also needs `qte_engine` beside the exe.

See also: [hci-dpi-checklist.md](hci-dpi-checklist.md).
