# 构建说明（M5）

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

## M5 验收

1. 按 **Alt** 显示 KeyTip 角标；输入字母激活 Tab/命令；**Esc** 退出。  
2. Home → **Styles** 组内可见横向 Gallery（Normal / Title / Quote）。  
3. 点击 Gallery 项触发对应 action。  
4. 既有 QAT / Backstage / Simplified 仍可用。

## 相关

- [architecture.md](architecture.md) · [dev-plan.md](dev-plan.md)
