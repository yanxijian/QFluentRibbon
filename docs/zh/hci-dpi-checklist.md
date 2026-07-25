# HCI / 高 DPI 检查清单（M6）

> **English**：[../en/hci-dpi-checklist.md](../en/hci-dpi-checklist.md)  
> **联调对象**：[QThemeEngine](https://github.com/yanxijian/QThemeEngine) Fluent light / dark / **hc** pack。

在改完 Ribbon 壳或度量后，按本清单在 **Windows + 真实 DPI** 上过一遍 Gallery（`qfr_gallery`）。

## 1. 环境矩阵

| 项 | 建议取值 |
|----|----------|
| 缩放 | 100% / 125% / 150% / 200%（至少测 100% 与 150%） |
| 皮肤 | Fluent Light / Dark / **High Contrast** |
| 窗口 | 最大化 ↔ 还原；拖到第二屏（若 DPI 不同） |

Qt 6 默认高 DPI；勿再设过时的 `AA_EnableHighDpiScaling`。

## 2. Ribbon 壳

| 检查 | 通过标准 |
|------|----------|
| QAT / Tab / 面板高度 | 各 DPI 下无裁切、无重叠 |
| 组标题 / 分隔线 | 清晰；HC 下对比足够 |
| Accent 下划线 | 对准当前 Tab，缩放后不「糊成一条」 |
| Simplified | 切换后高度与按钮档位正确，无残影 |
| Backstage | 覆盖中央区对齐；导航与内容可读 |
| KeyTip 角标 | Alt 后位置贴控件；Esc 可退 |
| ScreenTip | 悬停标题+说明完整，不被屏边裁掉过多 |

## 3. 命令与 Gallery

| 检查 | 通过标准 |
|------|----------|
| Large / Medium / Small | 缩窄窗口降档，拉宽恢复；图标不糊（优先矢量/多尺寸） |
| 组启动器 ⌟ | 可点；简化模式下隐藏 |
| In-Ribbon Gallery | 项可点；组内布局不挤爆 |
| QAT 钉/卸钉 | 图标清晰；持久化仍可用 |

## 4. 与 QTE hc pack

| 检查 | 通过标准 |
|------|----------|
| 切 High Contrast | `ribbon.*` 色随 palette；无依赖浅色硬编码 |
| 焦点环 | 原生 `QToolButton` 等仍由 QTE Style 处理；壳层自绘不盖住焦点 |
| 圆角 / 描边 | HC 下描边可见（QTE 常为零圆角） |

## 5. 记录

失败项记：DPI、皮肤、窗口尺寸、控件路径、截图。修复后回归本清单相关行即可。
