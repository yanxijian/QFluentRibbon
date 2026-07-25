# 构建说明（M2）

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

## M2 验收

1. Gallery 勾选 **Simplified ribbon**：条带变矮，组标题隐藏，命令不超过 Medium。  
2. 悬停命令：ScreenTip 显示标题 + 说明。  
3. 点击组右下角 ⌟：触发对话框启动器（Font 会弹出占位框）。  
4. 缩窄/拉宽窗口：档位随宽度变化且无崩溃。  
5. Skin 切换仍只靠 ThemeStore。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
