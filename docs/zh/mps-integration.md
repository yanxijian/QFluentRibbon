# MultiProcessShell 接入备忘（可选）

> **English**：[../en/mps-integration.md](../en/mps-integration.md)  
> **地位**：说明性备忘；**不**要求改 [MultiProcessShell](https://github.com/yanxijian/MultiProcessShell) 仓库。  
> **日期**：2026-07-25

## 目标关系

| 仓 | 角色 |
|----|------|
| QFluentRibbon | Host 窗口内的 Ribbon 命令条 |
| QThemeEngine | 皮肤 SSOT（Host 先 `Engine::apply`） |
| MultiProcessShell | 多进程壳；嵌入 Client 页 |

## 建议接入顺序（将来做时）

1. Host 进程：`qtheme::Engine::apply` → `ThemeBridge::bind` → `RibbonWindow` / `RibbonBar`。  
2. 将现有 Host 主窗改为 `RibbonWindow`，或把 `RibbonBar` 设为 `QMainWindow::setMenuWidget`。  
3. 中央区仍放 MPS 的 Tab/嵌入容器；Backstage 覆盖中央区即可，不必进 Client。  
4. 命令用 `QAction`：本进程动作直接槽；跨进程动作走现有 MPS IPC（不要把 IPC 塞进 QFR）。  
5. 链接：`find_package(QThemeEngine)` + `find_package(QFluentRibbon)`，或源码旁路 `add_subdirectory`。

## 明确不做（本期）

- 不在 QFR 内预埋 MPS 类型或 IPC。  
- 不把 Client 进程也链 QFR（除非该进程自己要 Ribbon）。  
- 不把皮肤状态经 MPS 同步到 Client（各进程各自 `Engine::apply`，或仅 Host 有 UI 壳）。

## 风险

| 风险 | 缓解 |
|------|------|
| 双主题通道 | Host 禁止 Ribbon/壳层 QSS；只认 ThemeStore |
| 焦点/嵌入 | Backstage / KeyTip 打开时注意勿抢 Client 嵌入焦点；可按需 `dismiss` |
| 安装依赖 | QFR 包依赖已安装的 QThemeEngine |
