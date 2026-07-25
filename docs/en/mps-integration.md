# MultiProcessShell integration notes (optional)

> **中文主文档**：[../zh/mps-integration.md](../zh/mps-integration.md)

QFluentRibbon is the Host-side Ribbon only. Apply QTE first, then host `RibbonWindow` / `RibbonBar`; keep MPS embed UI in the central area. Wire commands via `QAction` (local slots or existing MPS IPC). Do **not** bake MPS types into QFR. Package consumers need installed QThemeEngine + QFluentRibbon (`find_package`).
