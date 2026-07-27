# QFluentRibbon

[![CI](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml/badge.svg)](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Qt Widgets 上的 **Office-like Ribbon** 框架；**外观与换肤由 [QThemeEngine](https://github.com/yanxijian/QThemeEngine)（QTE）驱动**。  
交互与信息架构参考 [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)（WPF），**不**移植其代码。

> English overview：[docs/en/README.md](docs/en/README.md)

## 定位

| 层 | 负责方 |
|----|--------|
| 颜色 / 度量 / light·dark·hc / Accent | **QTE**（`qtheme::Engine` + ThemeStore + `QThemeStyle` / `api`） |
| Ribbon 结构与交互（Tab / Group / QAT / Backstage / KeyTip / 简化模式） | **本库** |

红线：不对 Ribbon 壳再搞一套 QSS；专用自绘件只读 ThemeStore。

## 状态

**M0–M6 主线已落地**（Tab/Group、缩略、简化、ScreenTip、QAT、Backstage、KeyTip、Gallery、`find_package` 导出）。  
HCI/DPI 抽测见 [检查清单](docs/zh/hci-dpi-checklist.md)。

## 快速开始（Windows）

```bat
:: vcvars x64；QTDIR + 已安装的 QThemeEngine（CMAKE_PREFIX_PATH）
:: 本地共享库惯例：构建目录 build-shared；PREFIX 常为三仓同级的 prefix/
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
ctest --test-dir build-shared --output-on-failure
build-shared\qfr_demo.exe
```

产物：动态库 `qfr_ribbon`，Demo `qfr_demo`，导入目标 `QFluentRibbon::ribbon`。  
细节：[docs/zh/build.md](docs/zh/build.md)。

## 文档

| 主题 | 中文 | English |
|------|------|---------|
| 架构（权威） | [architecture.md](docs/zh/architecture.md) | [architecture.md](docs/en/architecture.md) |
| 开发方案 / 计划 | [dev-plan.md](docs/zh/dev-plan.md) | [dev-plan.md](docs/en/dev-plan.md) |
| 构建 / 安装 | [build.md](docs/zh/build.md) | [build.md](docs/en/build.md) |
| HCI / 高 DPI | [hci-dpi-checklist.md](docs/zh/hci-dpi-checklist.md) | [hci-dpi-checklist.md](docs/en/hci-dpi-checklist.md) |

日常以中文文档为准。

## 要求

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets |
| QThemeEngine | 编 Demo（`QFR_BUILD_EXAMPLES`）时需已安装；本库 DLL **不**链接 QTE |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64 |

## 仓库布局

```text
include/qfluentribbon/   公共 API（C++ 命名空间 qfluentribbon）
src/                     实现（含 layout/collapse）
cmake/                   安装用 Config 模板
examples/gallery/        Demo（qfr_demo）
tests/                   布局 / QAT 单测
docs/zh|en/              中英文文档
```

## License

Released under the [MIT License](LICENSE).
