# Build (M0)

> **中文主文档**：[../zh/build.md](../zh/build.md)

## Dependencies

Qt 6.8+ Widgets; QThemeEngine via sibling `../QThemeEngine`, `-DQFR_QTE_SOURCE_DIR=`, or installed `find_package`.

```bat
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR%
cmake --build build
build\qfr_gallery.exe
```

M0: switch Skin combo — Ribbon strip colors follow QTE packs (no Ribbon QSS).
