# 构建说明

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets（单测另需 Test） |
| QThemeEngine | **仅 Demo**（`QFR_BUILD_EXAMPLES`）需要；库目标 `qfr_ribbon` **不**链接 QTE |
| 工具链 | CMake 3.21+、Ninja；Windows MSVC x64 |

## 推荐：本地 prefix

本地共享库惯例使用构建目录 **`build-shared`**。通过 `CMAKE_PREFIX_PATH` 找已安装的 QTE（`qte_engine`）。

```bat
:: PREFIX = 安装根（常与三仓同级的 prefix/）；需已安装 QTE
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DCMAKE_INSTALL_PREFIX=%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON -DQFR_BUILD_EXAMPLES=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
cmake --install build-shared
build-shared\qfr_demo.exe
```

Demo 通过 `find_package(QThemeEngine)` PRIVATE 链接；运行时旁路 `qfr_ribbon.dll` 与 `qte_engine.dll`。

| 产物 | 名称 |
|------|------|
| 动态库 | `qfr_ribbon` |
| Demo | `qfr_demo` |
| 单测 | `qfr_tests`、`qfr_qat_tests` |
| CMake 导入 | `QFluentRibbon::ribbon` |

## 消费方

```cmake
find_package(QFluentRibbon REQUIRED)
target_link_libraries(app PRIVATE QFluentRibbon::ribbon)
# 若用 QTE 驱动肤色，由应用自己 find_package(QThemeEngine) 并同步 ribbon_tokens
```
