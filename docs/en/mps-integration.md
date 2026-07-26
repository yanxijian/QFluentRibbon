# MultiProcessShell integration notes (optional)

> **中文主文档**：[../zh/mps-integration.md](../zh/mps-integration.md)  
> **Updated**：2026-07-26 (global appearance SSOT + Client embed path)

Two parallel integration paths:

**A — Host chrome：** Apply QTE, host `RibbonWindow` / `RibbonBar`, keep MPS embed UI in the central area. Wire commands via `QAction` (local slots or existing MPS IPC).

**B — Client page (MPS Demo)：** Frameless `RibbonWindow` pages inside `mps_demo_client`; Host links **QTE** for shell chrome (not full QFR Ribbon) and embeds via HWND/`SetParent`. **Appearance SSOT is Host**: sync Light/Dark with bidirectional `Invoke("theme.set")` (`"light"` / `"dark"`); Host pushes the current scheme right after handshake. Details: MultiProcessShell `docs/zh/qfr-demo-client.md`.

Do **not** bake MPS types or Host↔Client theme protocol into QFR (product/Demo owns `Invoke`). Default: install QTE then `find_package(QThemeEngine)` + `find_package(QFluentRibbon)`. Sibling embed is opt-in only (`QFR_DEV_EMBED_QTE=ON`, incompatible with `QFR_INSTALL`).
