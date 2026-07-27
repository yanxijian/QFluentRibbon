# QFluentRibbon

[![CI](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml/badge.svg)](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](../../LICENSE)

Office-like **Ribbon** for **Qt Widgets**, with **look & skinning driven by [QThemeEngine](https://github.com/yanxijian/QThemeEngine) (QTE)**.  
Interaction/IA is inspired by [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon) (WPF) — **not** a port.

> Canonical docs are Chinese — start at the [root README](../../README.md).

## Features

- **Structure & interaction**: Tab / Group, width collapse, simplified mode, ScreenTip, QAT, Backstage, KeyTip, Gallery
- **Skinning via QTE only**: colors / metrics / light·dark·hc / Accent from `qtheme::Engine` + ThemeStore — no private QSS
- **`QAction`-first commands**: prefer native `QToolButton` / `QMenu` under `QThemeStyle`
- **Product packaging**: `find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`; library `qfr_ribbon` does **not** link QTE (demo does)
- **Gallery demo**: `examples/gallery` (`qfr_demo`)

Layering:

| Layer | Owner |
|-------|--------|
| Colors / metrics / light·dark·hc / Accent | **QTE** |
| Ribbon structure & interaction | **This library** |

## Requirements

| Item | Notes |
|------|--------|
| Qt | **6.8+** Widgets |
| QThemeEngine | Required when building the demo (`QFR_BUILD_EXAMPLES`); library DLL does **not** link QTE |
| Toolchain | CMake 3.21+, Ninja; MSVC x64 (`vcvars`) on Windows |
| Optional | `clang-format` 20 for local format checks |

## Quick start (Windows)

```bat
:: vcvars x64; QTDIR set; QThemeEngine already installed into PREFIX
set QTDIR=<Qt-6.8+-prefix>
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
ctest --test-dir build-shared --output-on-failure
build-shared\qfr_demo.exe
```

Artifacts: shared library `qfr_ribbon`, demo `qfr_demo`, imported target `QFluentRibbon::ribbon`.  
See [build.md](build.md) and [ci.md](ci.md).

## Layout

```text
include/qfluentribbon/   Public API (namespace qfluentribbon)
src/                     Implementation (incl. layout/collapse)
cmake/                   Install Config templates
examples/gallery/        Demo (qfr_demo)
tests/                   Layout / QAT unit tests
docs/zh|en/              Chinese + English docs
```

## Documentation

| Topic | 中文（主） | English |
|-------|------------|---------|
| Architecture (canonical) | [../zh/architecture.md](../zh/architecture.md) | [architecture.md](architecture.md) |
| Dev plan | [../zh/dev-plan.md](../zh/dev-plan.md) | [dev-plan.md](dev-plan.md) |
| Build | [../zh/build.md](../zh/build.md) | [build.md](build.md) |
| HCI / high DPI | [../zh/hci-dpi-checklist.md](../zh/hci-dpi-checklist.md) | [hci-dpi-checklist.md](hci-dpi-checklist.md) |
| CI | [../zh/ci.md](../zh/ci.md) | [ci.md](ci.md) |

**Policy:** Prefer Chinese docs day-to-day. Product boundaries follow [architecture.md](../zh/architecture.md).

## Status

| Capability | Status |
|------------|--------|
| Engineering + QTE skinning seam (M0) | Done |
| Tab / Group / width-collapse MVP (M1) | Done |
| Simplified mode, ScreenTip, deeper collapse (M2) | Done |
| Quick Access Bar + prefs serialization (M3) | Done |
| Backstage (M4) | Done |
| KeyTip / Gallery (M5) | Done |
| `find_package` export + HCI/DPI checklist (M6) | Done |
| Near-term polish / real product integration | See [dev-plan](dev-plan.md) |

## License

Released under the [MIT License](../../LICENSE).
