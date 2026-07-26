# MultiProcessShell 接入备忘（可选）

> **English**：[../en/mps-integration.md](../en/mps-integration.md)  
> **地位**：说明性备忘；**不**要求在本仓预埋 MPS 类型。  
> **日期**：2026-07-26（全局外观 SSOT + Client 嵌入路径）

## 目标关系

| 仓 | 角色 |
|----|------|
| QFluentRibbon | Ribbon 命令条（可在 **Host 壳** 或 **Client 页**） |
| QThemeEngine | 皮肤引擎（各进程 `Engine::apply`） |
| MultiProcessShell | 多进程壳；嵌入 Client HWND；**Demo 外观 SSOT 在 Host** |

有两条并列接入路径，按产品目标选其一（可并存于不同进程）。

---

## 路径 A — Host 壳用 Ribbon（中央嵌普通 Client）

1. Host 进程：`qtheme::Engine::apply` → `ThemeBridge::bind` → `RibbonWindow` / `RibbonBar`。  
2. 将 Host 主窗改为 `RibbonWindow`，或把 `RibbonBar` 设为 `QMainWindow::setMenuWidget`。  
3. 中央区仍放 MPS 的 Tab/嵌入容器；Backstage 覆盖中央区即可，不必进 Client。  
4. 命令用 `QAction`：本进程动作直接槽；跨进程动作走现有 MPS IPC（不要把 IPC 塞进 QFR）。  
5. 链接：`find_package(QThemeEngine)` + `find_package(QFluentRibbon)`，或源码旁路 `add_subdirectory`。

## 路径 B — Client 页 = frameless Ribbon（MPS Demo 已落地）

Demo 侧：`mps_demo_client` 每张嵌入页是无系统标题栏的 `RibbonWindow`；Host 链 **QTE**（壳 chrome），**不**链完整 QFR Ribbon。

1. Client：`Engine::apply` → `ThemeBridge::bind`。  
2. 页构造：`Qt::FramelessWindowHint` + `WA_NativeWindow` → `show` → `winId` → `SubWindowAdded`。  
3. Host 仍 `SetParent` / 剥 caption（既有 `EmbedContainer`）。  
4. 不在 QFR 内引用 MPS 类型。  
5. **外观 SSOT 在 Host**：双向 `Invoke("theme.set")`（`"light"` / `"dark"`）同步 Host 壳与全部 Client；握手后 Host 立即推当前 scheme。

细节与嵌入态限制（KeyTip / Backstage）见 MPS 仓 `docs/zh/qfr-demo-client.md`。

---

## 明确不做

- 不在 QFR 内预埋 MPS 类型或 IPC。  
- 不在 QFR 内实现 Host↔Client 主题协议（由 MPS Demo / 产品层用现有 `Invoke` 承载）。

## 风险

| 风险 | 缓解 |
|------|------|
| 双主题通道 | 禁止壳层/页内私有 QSS 作为主题通道；只认 ThemeStore |
| 焦点/嵌入 | Backstage / KeyTip 打开时注意勿抢对端焦点；嵌入态 KeyTip 可能被裁切 |
| 安装依赖 | 包消费需已安装的 QThemeEngine；旁路源码时 sibling 布局即可 |
| 嵌入后改 flags | 勿在 `SetParent` 后再改会重建 HWND 的 `windowFlags` |
