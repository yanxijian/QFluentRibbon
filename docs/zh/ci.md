# CI（GitHub Actions）

> **English**：[../en/ci.md](../en/ci.md)

工作流：[`.github/workflows/ci.yml`](../../.github/workflows/ci.yml)

| Job | Runner | 内容 |
|-----|--------|------|
| `clang-format` | ubuntu | `clang-format` 20（PyPI）+ `scripts/format_source.py --check` |
| `Windows MSVC + Qt` | windows-latest | 拉取同级 QTE → 构建 QFR / `ctest` / gallery |
| `Ubuntu + Qt` | ubuntu-latest | 同上；`QT_QPA_PLATFORM=offscreen` 跑测 |

触发：`push`/`pull_request`，以及 `workflow_dispatch`。  
Dependabot：[`.github/dependabot.yml`](../../.github/dependabot.yml)。
