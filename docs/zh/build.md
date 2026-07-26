# 构建说明

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets |
| QThemeEngine | **仅 Gallery / 消费方 Demo** 需要；本库 DLL **不**链接 QTE |
| 工具链 | CMake 3.21+、Ninja；Windows MSVC x64 |

## 推荐：本地 prefix

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;D:\Codes\prefix ^
  -DCMAKE_INSTALL_PREFIX=D:\Codes\prefix ^
  -DQFR_BUILD_SHARED=ON -DQFR_INSTALL=ON -DQFR_BUILD_EXAMPLES=ON
cmake --build build
cmake --install build
```

Gallery 额外 `find_package(QThemeEngine)` 并 PRIVATE 链接；运行时旁路 `qfluentribbon.dll` + `qtheme_engine.dll`。

## 消费方

```cmake
find_package(QFluentRibbon REQUIRED)
target_link_libraries(app PRIVATE QFluentRibbon::qfluentribbon)
# 若用 QTE 驱动肤色，由应用自己 find_package(QThemeEngine) 并同步 ribbon_tokens
```
