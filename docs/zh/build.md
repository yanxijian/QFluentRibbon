# 构建说明（M0）

> **English**：[../en/build.md](../en/build.md)

## 依赖

| 项 | 说明 |
|----|------|
| Qt | 6.8+ Widgets |
| QThemeEngine | 默认使用同级目录 `../QThemeEngine`；或 `-DQFR_QTE_SOURCE_DIR=` / 已安装的 `find_package` |
| 工具链 | CMake 3.21+、Ninja；Windows 上 MSVC x64（`vcvars`） |

## Windows 快速构建

```bat
:: vcvars x64，QTDIR 指向 Qt 前缀
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=%QTDIR%
cmake --build build
build\qfr_gallery.exe
```

可选：

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR% ^
  -DQFR_QTE_SOURCE_DIR=D:\Codes\QThemeEngine ^
  -DQFR_BUILD_EXAMPLES=ON
```

## M0 验收

1. 启动 `qfr_gallery`：顶部可见 Ribbon 占位条（Tab 文案 + 面板）。  
2. 下方 Skin 下拉切换 Light / Dark / High Contrast：条带背景与强调色随之变化。  
3. 未对 Ribbon 使用 `setStyleSheet` 作为主题通道。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
