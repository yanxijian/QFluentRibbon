# CI (GitHub Actions)

> **中文主文档**：[../zh/ci.md](../zh/ci.md)

Workflow: [`.github/workflows/ci.yml`](../../.github/workflows/ci.yml)

| Job | Runner | Contents |
|-----|--------|----------|
| `clang-format` | ubuntu | clang-format 20 (PyPI) + `scripts/format_source.py --check` |
| `Windows MSVC + Qt` | windows-latest | Sibling QTE checkout → build QFR / `ctest` / gallery |
| `Ubuntu + Qt` | ubuntu-latest | Same; `QT_QPA_PLATFORM=offscreen` for tests |

Triggers: `push` / `pull_request` / `workflow_dispatch`.  
Dependabot: [`.github/dependabot.yml`](../../.github/dependabot.yml).
