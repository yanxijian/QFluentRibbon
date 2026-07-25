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

仓库刚建立，规格与开发方案见文档。实现按里程碑推进（见开发计划）。

## 文档

| 主题 | 中文 | English |
|------|------|---------|
| 架构（权威） | [architecture.md](docs/zh/architecture.md) | [architecture.md](docs/en/architecture.md) |
| 开发方案 / 计划 | [dev-plan.md](docs/zh/dev-plan.md) | [dev-plan.md](docs/en/dev-plan.md) |

日常以中文文档为准。

## 要求（规划）

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets |
| QTE | 同机构建或 `find_package(QThemeEngine)`（具体接入方式见架构文档） |
| 工具链 | CMake、Ninja；Windows 上优先 MSVC x64 |

## 仓库布局（规划）

```text
include/qfluentribbon/   公共 API
src/                     实现
examples/gallery/        Showcase（对标 Fluent.Ribbon Showcase 行为）
tests/                   规则 / 布局单测（优先无 GUI）
cmake/                   包配置
docs/zh|en/              中英文文档
```

## License

Released under the [MIT License](LICENSE).
