# 构建说明

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets（单测另需 Test） |
| QThemeEngine | **仅 Demo**（`QFR_BUILD_EXAMPLES`）需要；库目标 `qfr_ribbon` **不**链接 QTE |
| 工具链 | CMake 3.21+、Ninja；Windows MSVC x64 |

## 推荐：本地 prefix

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;D:\Codes\prefix ^
  -DCMAKE_INSTALL_PREFIX=D:\Codes\prefix ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON -DQFR_BUILD_EXAMPLES=ON -DQFR_BUILD_TESTS=ON
cmake --build build
cmake --install build
build\qfr_demo.exe
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
