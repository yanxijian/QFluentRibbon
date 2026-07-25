# 构建说明（M1）

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
ctest --test-dir build -C Release --output-on-failure
build\qfr_gallery.exe
```

可选：

```bat
cmake -S . -B build -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR% ^
  -DQFR_QTE_SOURCE_DIR=D:\Codes\QThemeEngine ^
  -DQFR_BUILD_EXAMPLES=ON ^
  -DQFR_BUILD_TESTS=ON
```

## M1 验收

1. 启动 `qfr_gallery`：顶部有真实 Tab（Home / Insert / View）与带标题的 Group。  
2. 点击命令：下方状态行显示 `Triggered: …`。  
3. 缩窄窗口：组内按钮由大图标+文案 → 中档 → 小档（图标或文字）。  
4. Skin 下拉切换 Light / Dark / High Contrast：条带与面板色随之变化。  
5. `qfr_tests` 通过（缩略规则 Large / Medium / Small）。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
