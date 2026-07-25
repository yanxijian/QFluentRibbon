# QFluentRibbon 架构

> **English**：[../en/architecture.md](../en/architecture.md)  
> **地位**：产品主路径与边界的权威说明。开发节奏见 [dev-plan.md](dev-plan.md)。  
> **更新**：2026-07-25（建仓）

---

## 1. 目标

在 **Qt Widgets** 上提供 Office-like **Ribbon** 框架：

1. **交互与结构**对齐业界成熟的 Ribbon 产品模型（参考 Fluent.Ribbon / MSDN Ribbon 指南）。
2. **外观与换肤**统一交给 **QThemeEngine（QTE）**，与应用内其它控件共用同一 ThemeStore。
3. 可被独立应用或后续 **MultiProcessShell** Host 集成，但不为集成做预重构。

## 2. 非目标

| 非目标 | 说明 |
|--------|------|
| 移植 Fluent.Ribbon | WPF/XAML 栈不可复用；只参考概念与行为 |
| 成为第二套主题引擎 | 禁止私有 QSS / 平行色表 |
| 像素级复刻 WinUI / Web Fluent | 以 QTE Fluent Pack + Office 交互为准 |
| 一次做完整个 Office 套件 | Backstage / KeyTip / QAT 等分期交付 |
| QML / Quick 版本 | 本仓库仅 Widgets |

## 3. 分层

```text
┌─────────────────────────────────────────┐
│  Application / Demo                      │
├─────────────────────────────────────────┤
│  QFluentRibbon  Shell                    │
│  RibbonWindow · TabBar · Groups · QAT … │
│  （布局、缩略、命令绑定、状态）            │
├─────────────────────────────────────────┤
│  组内命令控件                             │
│  优先：QToolButton / QMenu / QAction      │
│  必要时：自绘 + qtheme::api               │
├─────────────────────────────────────────┤
│  QThemeEngine                            │
│  Engine · ThemeStore · QThemeStyle       │
└─────────────────────────────────────────┘
```

| 层 | 职责 | 不职责 |
|----|------|--------|
| **QTE** | Pack、色/度量、Style、Accent、偏好 | Ribbon 布局语义、KeyTip 遍历 |
| **QFluentRibbon** | Ribbon 树、动态缩略、QAT/Backstage/KeyTip | 自建 skin 格式、全局 QSS |

## 4. 与参考库的概念映射

参考：[Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)（行为与信息架构，非 API 兼容层）。

| Fluent.Ribbon 概念 | 本库规划类型（暂定） | 备注 |
|--------------------|----------------------|------|
| `RibbonWindow` | `qfluentribbon::RibbonWindow` | 可先基于 `QMainWindow` |
| `Ribbon` | `RibbonBar` | 根条：tabs + 可选 QAT |
| `RibbonTabItem` / TabControl | `RibbonTab` | |
| `RibbonGroupBox` | `RibbonGroup` | Large / Medium / Small 缩略 |
| `QuickAccessToolBar` | `QuickAccessBar` | 分期 |
| `Backstage` | `Backstage` | 分期 |
| `Gallery` / `InRibbonGallery` | `RibbonGallery` | 后期 |
| `ScreenTip` / `KeyTip` | `ScreenTip` / `KeyTipService` | 后期 |
| `IsSimplified` | `RibbonBar::setSimplified` | 简化模式 |

命令绑定优先 **`QAction`**（与 Qt 生态一致），不强制 Fluent.Ribbon 的 `ICommand` 模型。

## 5. 主题合约（与 QTE）

### 5.1 原则

- `Engine::apply` 之后，Ribbon 与内容区控件共用同一 Store。
- 壳层与专用自绘：**只**通过 ThemeStore / `qtheme::api` 取色与度量。
- **禁止**对 Ribbon 壳 `setStyleSheet` 作为主题通道。

### 5.2 拟扩展的度量 / 色组（草案）

在 QTE Fluent Pack 中增加 `ribbon` 组（或本库提供 overlay Pack 合并进 Store），例如：

| 角色（草案） | 用途 |
|--------------|------|
| `ribbon/tab.height` | 标签行高度 |
| `ribbon/group.height` | 经典模式组高 |
| `ribbon/group.height.simplified` | 简化模式高度 |
| `ribbon/group.padding` | 组内边距 |
| `ribbon/icon.large` / `medium` / `small` | 图标档 |
| `ribbon/bg` / `ribbon/border` / `ribbon/tab.*` | 壳表面色（若 Style 未覆盖） |

具体键名在 M0 与 QTE 对齐时冻结；本库**不**维护平行 JSON 真源（可有 demo overlay，SSOT 仍归 QTE Pack 或明确的 merge 链）。

### 5.3 换肤

应用侧：

```cpp
qtheme::Engine engine;
engine.apply(&app);
// …
ribbon->polishFromStore(); // 或监听 Store generation / Engine 信号刷新壳
engine.switchSkin(QStringLiteral("dark"));
```

本库提供明确的「Store 变更 → 壳重测/重绘」路径，避免各控件私自缓存过期色。

## 6. 模块边界（规划目录）

```text
include/qfluentribbon/
  ribbon_bar.hpp
  ribbon_tab.hpp
  ribbon_group.hpp
  ribbon_window.hpp
  …（按里程碑增长）
src/
  layout/          # 缩略与行布局算法（尽量可单测）
  chrome/          # 壳绘制辅助
  …
examples/gallery/  # Showcase
tests/             # 布局/缩略规则优先无 GUI
```

**深模块优先**：缩略规则、组内 item 尺寸选择等纯逻辑放进可测单元，再接到 Widgets。

## 7. 构建与依赖（规划）

| 项 | 规划 |
|----|------|
| CMake | 独立工程；选项 `QFR_BUILD_EXAMPLES` / `QFR_BUILD_TESTS` |
| QTE | `find_package(QThemeEngine)` 或 `FetchContent` / 本地 path（开发初期可用 path） |
| Qt | 6.8+ Widgets |
| 平台 | 首期 Windows；布局逻辑保持平台无关 |

## 8. 验收视角

- **皮肤**：切换 QTE light/dark/hc（及 Accent）时，Ribbon 壳与组内原生控件同步变化，无第二套色。
- **交互**：经典模式下 Tab/Group/命令可点；缩略在窄宽度下可演示至少两档。
- **参考**：Showcase 行为可对照 Fluent.Ribbon Showcase（不要求像素一致）。

## 9. 相关文档

| 文档 | 角色 |
|------|------|
| [dev-plan.md](dev-plan.md) | 里程碑与近中期姿态 |
| QTE [architecture](https://github.com/yanxijian/QThemeEngine/blob/main/docs/zh/architecture.md) | 主题引擎权威 |
| Fluent.Ribbon README / docs | 交互参考（外部） |
