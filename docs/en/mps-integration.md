# MultiProcessShell integration notes (optional)

> **中文主文档**：[../zh/mps-integration.md](../zh/mps-integration.md)  
> **Updated**：2026-07-26 (Client embed path)

Two parallel integration paths:

**A — Host chrome：** Apply QTE, host `RibbonWindow` / `RibbonBar`, keep MPS embed UI in the central area. Wire commands via `QAction` (local slots or existing MPS IPC).

**B — Client page (MPS Demo)：** Frameless `RibbonWindow` pages inside `mps_demo_client`; Host embeds via existing HWND/`SetParent` and does not link QFR. Details: MultiProcessShell `docs/zh/qfr-demo-client.md`.

Do **not** bake MPS types into QFR. Do **not** sync skins over IPC. Package consumers need installed QThemeEngine + QFluentRibbon (`find_package`), or sibling source trees for day-to-day builds.
