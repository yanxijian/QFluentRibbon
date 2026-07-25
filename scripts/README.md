# Scripts

| Script | Role |
|--------|------|
| `format_source.py` | Format C++ with root `.clang-format` (skips `build/` / `_deps/` / `*_autogen/`) |

```bat
python scripts/format_source.py
python scripts/format_source.py --check
```
