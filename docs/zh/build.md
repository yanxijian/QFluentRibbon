# 构建说明（M6）

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets（测试需 Test） |
| QThemeEngine | 默认同级 `../QThemeEngine`；或 `-DQFR_QTE_SOURCE_DIR=` / 已安装的 `find_package` |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64（`vcvars`） |

## Windows 快速构建

```bat
:: vcvars x64，QTDIR 指向 Qt 前缀
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR% -DQFR_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
build\qfr_gallery.exe
```

## 安装与 `find_package`

旁路开发（同级 `../QThemeEngine`）：保持 `-DQFR_INSTALL=OFF`（默认）。

可安装包需 **先安装 QTE**，再用 `find_package(QThemeEngine)` 配置本库：

```bat
cmake --install path\to\QThemeEngine\build --prefix D:\prefix
cmake -S . -B build-install -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;D:\prefix ^
  -DQFR_QTE_SOURCE_DIR= ^
  -DQFR_INSTALL=ON
cmake --build build-install
cmake --install build-install --prefix D:\prefix
```

消费方：

```cmake
find_package(QThemeEngine REQUIRED)
find_package(QFluentRibbon REQUIRED)
target_link_libraries(app PRIVATE QFluentRibbon::qfluentribbon)
```

## M6 验收

1. `-DQFR_INSTALL=ON` 可配置；`cmake --install` 产出 headers + cmake 包。  
2. 文档：[HCI/DPI 清单](hci-dpi-checklist.md)、[MPS 接入备忘](mps-integration.md)。  
3. Gallery 在 light/dark/hc 与至少两种 DPI 下按清单抽测。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md) · [hci-dpi-checklist.md](hci-dpi-checklist.md) · [mps-integration.md](mps-integration.md)
