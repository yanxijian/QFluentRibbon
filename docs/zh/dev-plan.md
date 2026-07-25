# QFluentRibbon 开发方案

> **English**：[../en/dev-plan.md](../en/dev-plan.md)  
> **地位**：建仓后的开发方案与里程碑备忘；**产品边界以 [architecture.md](architecture.md) 为准**。  
> **更新**：2026-07-25（M0 工程骨架落地：CMake↔QTE、ThemeBridge、RibbonBar 占位）

---

## 1. 一句话目标

做 **Qt Widgets 上的 Office-like Ribbon 框架**，**皮肤由 QTE 驱动**；Ribbon 风格界面与交互参考 Fluent.Ribbon，**不移植** WPF 实现。

## 2. 当前姿态

| 结论 | 说明 |
|------|------|
| **M0 已完成** | CMake 链 QTE；`ThemeBridge` 播种 `ribbon.*`；`RibbonBar`/`RibbonWindow` 占位；`qfr_gallery` 可切肤 |
| **双依赖、单皮肤源** | 功能依赖 Qt Widgets + QTE；视觉 SSOT 只认 ThemeStore |
| **参考而非端口** | Fluent.Ribbon 提供信息架构与行为清单；API 按 Qt/`QAction` 习惯设计 |
| **下一刀：M1** | Tab / Group / QAction + 基础缩略 |

### M0 出站核对

| 项 | 状态 |
|----|------|
| CMake `../QThemeEngine` / `QFR_QTE_SOURCE_DIR` / `find_package` | 已接 |
| `Engine::apply` + light/dark/hc 切换条带变色 | Gallery 验收 |
| `ribbon.*` 草案（Bridge 播种，键名见 ThemeBridge） | 已文档化于构建说明 / 架构 |
| 无 Ribbon 私有 QSS | 遵守 |

## 3. 原则（红线）

1. **禁止** Ribbon 私有 QSS 主题通道。  
2. 专用自绘只读 **ThemeStore / `qtheme::api`**。  
3. 组内命令控件**优先原生**（`QToolButton` / `QMenu` 等）吃 `QThemeStyle`。  
4. 缩略 / 简化等规则尽量 **纯逻辑 + 单测**，再绑 Widgets。  
5. **不为** MPS 集成预重构；集成当天再接。  
6. 不把 Web Fluent tokens / 第三方 QML FluentUI 当 Windows 像素真源。

## 4. 里程碑

### M0 — 工程与换肤缝（约 1 个迭代）

| 交付 | 验收 |
|------|------|
| CMake 工程、`find_package`/path 接入 QTE | Demo 能 `Engine::apply` + 切 skin |
| 空 `RibbonWindow` + 占位 `RibbonBar` | 窗口内可见条带；切 light/dark 条带色变 |
| `ribbon.*` 度量/色草案合入 QTE Pack 或 overlay | 键名文档化；Showcase 只读 Store |
| 文档：构建说明 stub | README 可按步骤跑通 |

**出站标准**：换肤路径打通，尚无完整 Tab 交互也可。

### M1 — 经典 Ribbon MVP

| 交付 | 验收 |
|------|------|
| `RibbonTab` / `RibbonGroup` | 多 Tab 切换；组标题可见 |
| 组内 `QAction` → 大/中/小按钮 | 点击触发 action |
| 基础宽度缩略（≥2 档） | 缩窄窗口时组内尺寸降档，可单测规则 |
| Gallery 示例页 | 对照 Fluent.Ribbon 的「有 Tab/Group/命令」最小集 |

**出站标准**：可当作「能用的命令条」嵌入示例应用。

### M2 — 壳体验加深

| 交付 | 验收 |
|------|------|
| 简化模式 `setSimplified` | 经典 ↔ 简化可切换 |
| 组启动器 / 对话框按钮（可选） | 行为接近 Office「组右下角」 |
| ScreenTip（富文本提示，轻量） | 悬停有标题+说明 |
| 更多缩略策略与回归测 | 窄/宽往复无崩溃、无残影 |

### M3 — QAT

| 交付 | 验收 |
|------|------|
| `QuickAccessBar` | 可钉住常用 action |
| 位置：标题栏下 / 条上（先做一种，第二种可选） | Showcase 可演示 |
| 状态序列化（QSettings） | 重启后保留 |

### M4 — Backstage

| 交付 | 验收 |
|------|------|
| `Backstage` 全屏/覆盖式面板 | 打开/关闭动画可无或简单 |
| 左侧导航 + 内容页 | 可放「新建/打开」占位页 |

### M5 — KeyTip 与 Gallery

| 交付 | 验收 |
|------|------|
| Alt KeyTip 遍历 | 与 Tab/Group/按钮可达 |
| `RibbonGallery` / In-Ribbon Gallery（子集） | Showcase 一页足够 |

### M6 — 打磨与集成

| 交付 | 验收 |
|------|------|
| HCI / 高 DPI 检查清单 | 与 QTE hc pack 联调 |
| `find_package(QFluentRibbon)` 安装导出 | 外部工程可链接 |
| （可选）MPS Host 接入备忘 | 只写集成说明，不强制改 MPS |

## 5. 建议落地顺序（近两周）

```text
M0 骨架 + QTE 换肤
  → M1 Tab/Group/Action + 两档缩略 + Gallery
    → （并行）QTE Pack 补 ribbon.* 键
      → M2 简化模式
        → 再排 QAT / Backstage
```

## 6. 明确暂不做

| 项 | 原因 |
|----|------|
| 兼容 Fluent.Ribbon 的 C# API | 无价值，徒增扭曲 |
| 自研完整主题格式 | 与 QTE 重复 |
| macOS/Linux 首期像素打磨 | 布局逻辑可跨平台，视觉首期 Windows |
| Ribbon 内嵌 WebEngine / 复杂 Gallery 数据虚化 | 超出 MVP |
| 与 FluentUI（QML）共用代码 | 栈不同 |

## 7. 风险与缓解

| 风险 | 缓解 |
|------|------|
| 做成第二主题引擎 | 代码审：禁止壳层 QSS；色来源 grep ThemeStore |
| 缩略逻辑与 UI 缠死 | `src/layout` 纯函数 + tests |
| QTE API 变动 | M0 用薄适配层 `ThemeBridge`，只暴露本库需要的色/度量查询 |
| 范围膨胀 | 里程碑出站标准写进 PR / Issue；Showcase 不超前堆功能 |

## 8. 协作与依赖仓

| 仓 | 关系 |
|----|------|
| [QThemeEngine](https://github.com/yanxijian/QThemeEngine) | **必须**：皮肤与度量 |
| [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)（只读参考） | 交互/IA 对照 |
| [MultiProcessShell](https://github.com/yanxijian/MultiProcessShell) | 远期 Host 可选集成 |

## 9. 相关文档

| 文档 | 角色 |
|------|------|
| [architecture.md](architecture.md) | 边界与分层（权威） |
| 本文件 | 里程碑与近中期方案 |
| QTE 架构 / Pack | 皮肤 SSOT |
