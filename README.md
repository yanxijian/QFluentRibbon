# QFluentRibbon

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

**M2 已落地**：简化模式、组启动器、ScreenTip-lite、缩略 `maxSize` 回归。  
下一阶段 **M3**（QAT）见 [开发方案](docs/zh/dev-plan.md)。

## 快速开始（Windows）

```bat
:: vcvars x64，设置 QTDIR；默认使用同级 ../QThemeEngine
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR% -DQFR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
build\qfr_gallery.exe
```

细节：[docs/zh/build.md](docs/zh/build.md)。

## 文档

| 主题 | 中文 | English |
|------|------|---------|
| 架构（权威） | [architecture.md](docs/zh/architecture.md) | [architecture.md](docs/en/architecture.md) |
| 开发方案 / 计划 | [dev-plan.md](docs/zh/dev-plan.md) | [dev-plan.md](docs/en/dev-plan.md) |
| 构建 | [build.md](docs/zh/build.md) | [build.md](docs/en/build.md) |

日常以中文文档为准。

## 要求

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets |
| QTE | 默认 `../QThemeEngine`，或 `-DQFR_QTE_SOURCE_DIR=` / `find_package` |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64 |

## 仓库布局

```text
include/qfluentribbon/   公共 API（Bar / Tab / Group / ThemeBridge）
src/                     实现（含 layout/collapse）
examples/gallery/        M1 Showcase（Tab / 命令 / 缩略 / 换肤）
tests/                   缩略规则单测
docs/zh|en/              中英文文档
```

## License

Released under the [MIT License](LICENSE).
