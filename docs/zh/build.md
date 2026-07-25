# 构建说明（M3）

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets（测试需 Test） |
| QThemeEngine | 默认使用同级目录 `../QThemeEngine`；或 `-DQFR_QTE_SOURCE_DIR=` / 已安装的 `find_package` |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64（`vcvars`） |

## Windows 快速构建

```bat
:: vcvars x64，QTDIR 指向 Qt 前缀
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR% -DQFR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
build\qfr_gallery.exe
```

## M3 验收

1. Gallery 顶部 Tab 上方可见 QAT 图标条（默认 Paste / Bold，或上次保存的集合）。  
2. 用「Pin to QAT」钉上 Copy / Grid；右键 QAT 图标可卸钉。  
3. 关闭再开 Gallery：钉住集合经 `QSettings` 恢复。  
4. Skin / Simplified 仍可用。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
