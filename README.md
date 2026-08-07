# QFluentRibbon

[![CI](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml/badge.svg)](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Qt Widgets 上的 **Office-like Ribbon** 框架；**外观与换肤由 [QThemeEngine](https://github.com/yanxijian/QThemeEngine)（QTE）驱动**。  
交互与信息架构参考 [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)（WPF），**不**移植其代码。

> English overview：[docs/en/README.md](docs/en/README.md)

## 特性

- **结构与交互**：Tab / Group、宽度缩略、简化模式、ScreenTip、QAT、Backstage、KeyTip、Gallery
- **皮肤只认 QTE**：颜色 / 度量 / light·dark·hc / Accent 由 `qtheme::Engine` + ThemeStore 提供；本库不另搞 QSS
- **命令优先 `QAction`**：组内控件优先原生 `QToolButton` / `QMenu`，吃同一套 `QThemeStyle`
- **产品集成**：`find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`；库目标 `qfr_ribbon` **不**链接 QTE（Demo 才依赖）
- **画廊 Demo**：`examples/gallery`（`qfr_demo`）

分层约定：

| 层 | 负责方 |
|----|--------|
| 颜色 / 度量 / light·dark·hc / Accent | **QTE** |
| Ribbon 结构与交互 | **本库** |

## 要求

| 项 | 说明 |
|----|------|
| Qt | **6.8+** Widgets |
| [QThemeEngine](https://github.com/yanxijian/QThemeEngine) | 编 Demo（`QFR_BUILD_EXAMPLES`）时需已安装；本库 DLL **不**链接 QTE |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64（`vcvars`） |
| 可选 | `clang-format` 20（本地格式检查） |

## 快速开始（Windows）

1. 打开 **x64 Native Tools / vcvars** 环境，并设置 `QTDIR`。  
2. 确保本地 prefix 已安装 [QThemeEngine](https://github.com/yanxijian/QThemeEngine)（或先跑 [MultiProcessShell](https://github.com/yanxijian/MultiProcessShell) 的 `install_stack.py`）。  
3. 配置并构建（共享库惯例目录 **`build-shared`**）：

```bat
:: QTDIR = Qt 6.8+ 前缀；PREFIX = 安装根（常与三仓同级的 prefix/）
set QTDIR=<Qt-6.8+-prefix>
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
ctest --test-dir build-shared --output-on-failure
build-shared\qfr_demo.exe
```

产物：动态库 `qfr_ribbon`，Demo `qfr_demo`，导入目标 `QFluentRibbon::ribbon`。  
更细的构建选项见 [docs/zh/build.md](docs/zh/build.md)。CI：[docs/zh/ci.md](docs/zh/ci.md)。

## 仓库布局

```text
include/qfluentribbon/   公共 API（C++ 命名空间 qfluentribbon）
src/                     实现（含 layout/collapse）
cmake/                   安装用 Config 模板
examples/gallery/        Demo（qfr_demo）
tests/                   布局 / QAT 单测
docs/zh|en/              中英文文档
```

## 文档

| 主题 | 中文（主） | English |
|------|------------|---------|
| 架构（权威） | [architecture.md](docs/zh/architecture.md) | [architecture.md](docs/en/architecture.md) |
| 开发计划 | [dev-plan.md](docs/zh/dev-plan.md) | [dev-plan.md](docs/en/dev-plan.md) |
| 构建 | [build.md](docs/zh/build.md) | [build.md](docs/en/build.md) |
| HCI / 高 DPI | [hci-dpi-checklist.md](docs/zh/hci-dpi-checklist.md) | [hci-dpi-checklist.md](docs/en/hci-dpi-checklist.md) |
| CI | [ci.md](docs/zh/ci.md) | [ci.md](docs/en/ci.md) |

**约定**：日常以中文文档为准；英文为同步译本。产品边界以 [architecture.md](docs/zh/architecture.md) 为准。

## 现状

| 能力 | 状态 |
|------|------|
| 工程与 QTE 换肤缝（M0） | 完成 |
| Tab / Group / 宽度缩略 MVP（M1） | 完成 |
| 简化模式、ScreenTip、缩略加深（M2） | 完成 |
| Quick Access Bar + 偏好序列化（M3） | 完成 |
| Backstage（M4） | 完成 |
| KeyTip / Gallery（M5） | 完成 |
| `find_package` 导出 + HCI/DPI 清单（M6） | 完成 |
| 近中期打磨 / 真实产品接入 | 见 [dev-plan](docs/zh/dev-plan.md) |

## License

Released under the [MIT License](LICENSE).
