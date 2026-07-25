# QFluentRibbon architecture

> **中文主文档**：[../zh/architecture.md](../zh/architecture.md)  
> Canonical text is Chinese; this is a synced summary.  
> **Updated**: 2026-07-25 (repo bootstrap)

## Goal

Office-like Ribbon on **Qt Widgets**:

- Structure/interaction inspired by Fluent.Ribbon / MSDN Ribbon guidance.
- **Appearance & skinning owned by QTE** (shared ThemeStore with the rest of the app).
- Integrable later into MultiProcessShell Host — no pre-refactor for that.

## Non-goals

- Porting Fluent.Ribbon code (WPF).
- A second theme engine or Ribbon-private QSS.
- Pixel-matching WinUI / Web Fluent tokens.
- Shipping full Office surface in one milestone (QAT / Backstage / KeyTip are phased).

## Layers

```text
App → QFluentRibbon shell (layout, collapse, commands)
    → Prefer native QToolButton/QMenu (QThemeStyle)
    → Owner-draw chrome via qtheme::api when needed
    → QThemeEngine (Engine / Store / Style)
```

## Theme contract

- After `Engine::apply`, Ribbon and content share one store.
- No `setStyleSheet` as the theme channel for the shell.
- Planned `ribbon.*` metrics/colors live in QTE Fluent packs (or an explicit merge overlay); SSOT stays with QTE.

## See also

[dev-plan.md](dev-plan.md) for milestones.
