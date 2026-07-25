# QFluentRibbon development plan

> **中文主文档**：[../zh/dev-plan.md](../zh/dev-plan.md)  
> Canonical text is Chinese; this is a synced summary.  
> **Updated**: 2026-07-25 (M6 polish / export)

## Goal

Qt Widgets Office-like Ribbon framework; **skins driven by QTE**; Fluent.Ribbon as **behavioral reference only**.

## Posture

- **M6 done**: `find_package(QFluentRibbon)` install export; HCI/DPI checklist; optional MPS integration notes.
- M0–M6 mainline complete; further work is polish and product embedding.
- Single visual SSOT: ThemeStore. No parallel QSS theme.

## Milestones (short)

| ID | Focus |
|----|--------|
| **M0** | CMake + QTE wiring; placeholder `RibbonBar`; `ribbon.*` draft keys; skin switch works |
| **M1** | Tabs / groups / `QAction` buttons; ≥2 collapse tiers; gallery sample |
| **M2** | Simplified mode; optional group launcher; ScreenTip-lite |
| **M3** | Quick Access Bar + QSettings persistence |
| **M4** | Backstage panel |
| **M5** | KeyTips + gallery subset |
| **M6** | HC/DPI polish; `find_package` export; optional MPS notes |

## Red lines

No Ribbon-private QSS; owner-draw reads ThemeStore only; prefer native widgets in groups; layout rules unit-tested where possible; no MPS pre-refactor.

## Related

[architecture.md](architecture.md) · [hci-dpi-checklist.md](hci-dpi-checklist.md) · [mps-integration.md](mps-integration.md) · [QThemeEngine](https://github.com/yanxijian/QThemeEngine)
