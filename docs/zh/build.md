# 构建说明（M4）

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

## M4 验收

1. Gallery 点 **File (Backstage)**：中央区出现覆盖面板。  
2. 左侧切换 New / Open / Info；右侧内容随之切换。  
3. ← 或 Esc 关闭 Backstage。  
4. QAT / Simplified / Skin 仍可用。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
