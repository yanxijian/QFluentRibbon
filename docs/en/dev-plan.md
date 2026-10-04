# QFluentRibbon development plan

> **中文主文档**：[../zh/dev-plan.md](../zh/dev-plan.md)  
> Canonical text is Chinese; this is a synced summary.  
> **Updated**: 2026-10-04 (deferred items verified against tree)

## Goal

Qt Widgets Office-like Ribbon framework; **skins driven by QTE**; Fluent.Ribbon as **behavioral reference only**.

## Posture

- **M6 done**: `find_package(QFluentRibbon)` install export (`QFluentRibbon::ribbon`); HCI/DPI checklist.
- M0–M6 mainline complete; further work is polish and product embedding.
- Single visual SSOT: ThemeStore. No parallel QSS theme.

## Milestones (short)

| ID | Focus |
|----|--------|
| **M0** | CMake + QTE wiring; placeholder `RibbonBar`; `ribbon.*` draft keys; skin switch works |
| **M1** | Tabs / groups / `QAction` buttons; ≥2 collapse tiers; demo sample |
| **M2** | Simplified mode; optional group launcher; ScreenTip-lite |
| **M3** | Quick Access Bar + QSettings persistence |
| **M4** | Backstage panel |
| **M5** | KeyTips + gallery subset |
| **M6** | HC/DPI polish; `find_package` export |

## Red lines

No Ribbon-private QSS; owner-draw reads ThemeStore only; prefer native widgets in groups; layout rules unit-tested where possible; no pre-refactor for a specific host product.

## Deferred (verified, later)

| Item | Fact | Later |
|------|------|-------|
| Collapse widget tests | `test_collapse.cpp` covers `chooseUniformSizes` only, not a live `RibbonBar` | Width-matrix widget tests: no `LayoutRequest` loops, monotonic tiers |
| Simplified vs check state | **Done (2026-10-04)**: `setExclusiveActions` + `qfr_group_action_tests` | Width-matrix widget tests still open |

Rule-level collapse tests already exist — do not treat that as a gap.


## Related

[architecture.md](architecture.md) · [hci-dpi-checklist.md](hci-dpi-checklist.md) · [QThemeEngine](https://github.com/yanxijian/QThemeEngine)
