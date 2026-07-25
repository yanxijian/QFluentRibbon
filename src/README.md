# Source layout

See [docs/zh/dev-plan.md](../docs/zh/dev-plan.md).

| File | Role |
|------|------|
| `theme_bridge.cpp` | Seed/sync `ribbon.*` from QTE store; skin signals |
| `ribbon_bar.cpp` | M0 chrome placeholder (ThemeStore paint, no QSS) |
| `ribbon_window.cpp` | `QMainWindow` + north `RibbonBar` |

Later: `layout/` (collapse rules), tabs/groups, QAT / Backstage.
