# QFluentRibbon

**日本語** · [English](README.md) · [简体中文](README.zh-CN.md) · [한국어](README.ko.md)

<!-- translated-from: README.md sha256:68257f24838d -->

[![CI](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml/badge.svg)](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Qt Widgets** 向けの Office 風 **リボン**。見た目とスキンは **[QThemeEngine](https://github.com/yanxijian/QThemeEngine)（QTE）が駆動**します。
操作感と情報設計は [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)（WPF）を参考にしていますが、**移植ではありません**。

## 特長

- **構造と操作**：タブ／グループ、幅の折りたたみ、簡易表示モード、スクリーンチップ、クイックアクセスツールバー、バックステージ、キーチップ、ギャラリー
- **スキンは QTE 経由のみ**：色・寸法・ライト／ダーク／ハイコントラスト・アクセントカラーは `qtheme::Engine` と ThemeStore から取得し、独自 QSS は使いません
- **`QAction` を第一に**：`QThemeStyle` の下でネイティブの `QToolButton` / `QMenu` を優先します
- **製品としての配布**：`find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`。ライブラリ `qfr_ribbon` は QTE を**リンクしません**（デモはリンクします）
- **ギャラリーのデモ**：`examples/gallery`（`qfr_demo`）

レイヤーの分担：

| レイヤー | 担当 |
|-------|--------|
| 色・寸法・ライト／ダーク／ハイコントラスト・アクセントカラー | **QTE** |
| リボンの構造と操作 | **このライブラリ** |

## 動作要件

| 項目 | 内容 |
|------|--------|
| Qt | **6.8 以降** Widgets |
| [QThemeEngine](https://github.com/yanxijian/QThemeEngine) | デモをビルドするとき（`QFR_BUILD_EXAMPLES`）に必要。ライブラリ DLL は QTE を**リンクしません** |
| ツールチェーン | CMake 3.21 以降、Ninja。Windows では MSVC x64（`vcvars`） |
| 任意 | ローカルの書式チェック用に `clang-format` 20 |

## クイックスタート（Windows）

先に [QThemeEngine](https://github.com/yanxijian/QThemeEngine) を `PREFIX` に入れておきます（または [MultiProcessShell](https://github.com/yanxijian/MultiProcessShell) の `install_stack.py` を実行）。

```bat
:: vcvars x64; QTDIR を設定; QThemeEngine は既に PREFIX に入っている
set QTDIR=<Qt-6.8+-prefix>
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
ctest --test-dir build-shared --output-on-failure
build-shared\qfr_demo.exe
```

生成物：共有ライブラリ `qfr_ribbon`、デモ `qfr_demo`、インポートターゲット `QFluentRibbon::ribbon`。
詳しくは [build.md](docs/en/build.md) と [ci.md](docs/en/ci.md) を参照してください。

## リポジトリ構成

```text
include/qfluentribbon/   公開 API（名前空間 qfluentribbon）
src/                     実装（レイアウト／折りたたみを含む）
cmake/                   インストール用 Config テンプレート
examples/gallery/        デモ（qfr_demo）
tests/                   レイアウトとクイックアクセスツールバーの単体テスト
docs/zh|en/              中国語と英語のドキュメント
```

## ドキュメント

| 内容 | English | 简体中文 |
|------|---------|----------|
| アーキテクチャ（正） | [architecture.md](docs/en/architecture.md) | [architecture.md](docs/zh/architecture.md) |
| 開発計画 | [dev-plan.md](docs/en/dev-plan.md) | [dev-plan.md](docs/zh/dev-plan.md) |
| ビルド | [build.md](docs/en/build.md) | [build.md](docs/zh/build.md) |
| HCI / 高 DPI | [hci-dpi-checklist.md](docs/en/hci-dpi-checklist.md) | [hci-dpi-checklist.md](docs/zh/hci-dpi-checklist.md) |
| CI | [ci.md](docs/en/ci.md) | [ci.md](docs/zh/ci.md) |

長文の設計メモは英語と簡体字中国語の 2 言語のみで管理しています。この README が 4 言語で同期を保つ文書です。

**方針**：製品としての境界は [architecture.md](docs/zh/architecture.md) に従います。

## ステータス

| 機能 | 状態 |
|------|------|
| 基盤と QTE スキンの接合部（M0） | 完了 |
| タブ／グループ／幅の折りたたみの MVP（M1） | 完了 |
| 簡易表示モード、スクリーンチップ、より深い折りたたみ（M2） | 完了 |
| クイックアクセスツールバーと設定のシリアライズ（M3） | 完了 |
| バックステージ（M4） | 完了 |
| キーチップ／ギャラリー（M5） | 完了 |
| `find_package` のエクスポートと HCI/DPI チェックリスト（M6） | 完了 |
| 仕上げと実製品への統合 | [dev-plan](docs/en/dev-plan.md) を参照 |

## ライセンス

[MIT License](LICENSE) で公開しています。
