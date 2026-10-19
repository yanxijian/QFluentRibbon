# QFluentRibbon

**한국어** · [English](README.md) · [简体中文](README.zh-CN.md) · [日本語](README.ja.md)

<!-- translated-from: README.md sha256:68257f24838d -->

[![CI](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml/badge.svg)](https://github.com/yanxijian/QFluentRibbon/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Qt Widgets**용 Office 스타일 **리본**. 겉모습과 스킨은 **[QThemeEngine](https://github.com/yanxijian/QThemeEngine)(QTE)이 주도**합니다.
조작감과 정보 구조는 [Fluent.Ribbon](https://github.com/fluentribbon/Fluent.Ribbon)(WPF)을 참고했지만 **이식은 아닙니다**.

## 기능

- **구조와 조작**: 탭/그룹, 너비 접기, 간소화 모드, 스크린 팁, 빠른 실행 도구 모음, 백스테이지, 키 팁, 갤러리
- **스킨은 QTE를 통해서만**: 색, 치수, 라이트/다크/고대비, 강조색을 `qtheme::Engine`과 ThemeStore에서 받아오며 자체 QSS를 쓰지 않습니다
- **`QAction` 우선**: `QThemeStyle` 아래에서 네이티브 `QToolButton` / `QMenu`를 우선합니다
- **제품으로 배포**: `find_package(QFluentRibbon)` → `QFluentRibbon::ribbon`. 라이브러리 `qfr_ribbon`은 QTE를 **링크하지 않습니다**(데모는 링크)
- **갤러리 데모**: `examples/gallery`(`qfr_demo`)

계층 분담:

| 계층 | 담당 |
|-------|--------|
| 색, 치수, 라이트/다크/고대비, 강조색 | **QTE** |
| 리본 구조와 조작 | **이 라이브러리** |

## 요구 사항

| 항목 | 내용 |
|------|--------|
| Qt | **6.8 이상** Widgets |
| [QThemeEngine](https://github.com/yanxijian/QThemeEngine) | 데모를 빌드할 때(`QFR_BUILD_EXAMPLES`) 필요. 라이브러리 DLL은 QTE를 **링크하지 않습니다** |
| 툴체인 | CMake 3.21 이상, Ninja. Windows에서는 MSVC x64(`vcvars`) |
| 선택 | 로컬 서식 검사용 `clang-format` 20 |

## 빠른 시작(Windows)

먼저 [QThemeEngine](https://github.com/yanxijian/QThemeEngine)을 `PREFIX`에 설치합니다(또는 [MultiProcessShell](https://github.com/yanxijian/MultiProcessShell)의 `install_stack.py` 실행).

```bat
:: vcvars x64; QTDIR 설정; QThemeEngine은 이미 PREFIX에 설치됨
set QTDIR=<Qt-6.8+-prefix>
set PREFIX=<install-prefix>
cmake -S . -B build-shared -G Ninja ^
  -DCMAKE_PREFIX_PATH=%QTDIR%;%PREFIX% ^
  -DQFR_BUILD_SHARED=ON -DQFR_BUILD_TESTS=ON
cmake --build build-shared
ctest --test-dir build-shared --output-on-failure
build-shared\qfr_demo.exe
```

산출물: 공유 라이브러리 `qfr_ribbon`, 데모 `qfr_demo`, 임포트 타깃 `QFluentRibbon::ribbon`.
자세한 내용은 [build.md](docs/en/build.md)와 [ci.md](docs/en/ci.md)를 보세요.

## 저장소 구조

```text
include/qfluentribbon/   공개 API(네임스페이스 qfluentribbon)
src/                     구현(레이아웃/접기 포함)
cmake/                   설치용 Config 템플릿
examples/gallery/        데모(qfr_demo)
tests/                   레이아웃과 빠른 실행 도구 모음 단위 테스트
docs/zh|en/              중국어와 영어 문서
```

## 문서

| 주제 | English | 简体中文 |
|------|---------|----------|
| 아키텍처(기준) | [architecture.md](docs/en/architecture.md) | [architecture.md](docs/zh/architecture.md) |
| 개발 계획 | [dev-plan.md](docs/en/dev-plan.md) | [dev-plan.md](docs/zh/dev-plan.md) |
| 빌드 | [build.md](docs/en/build.md) | [build.md](docs/zh/build.md) |
| HCI / 고 DPI | [hci-dpi-checklist.md](docs/en/hci-dpi-checklist.md) | [hci-dpi-checklist.md](docs/zh/hci-dpi-checklist.md) |
| CI | [ci.md](docs/en/ci.md) | [ci.md](docs/zh/ci.md) |

긴 설계 문서는 영어와 중국어 간체 두 언어로만 관리합니다. 이 README가 네 언어 사이에서 동기화를 유지하는 문서입니다.

**방침**: 제품 경계는 [architecture.md](docs/zh/architecture.md)를 따릅니다.

## 현재 상태

| 기능 | 상태 |
|------|------|
| 기반과 QTE 스킨 접합부(M0) | 완료 |
| 탭/그룹/너비 접기 MVP(M1) | 완료 |
| 간소화 모드, 스크린 팁, 더 깊은 접기(M2) | 완료 |
| 빠른 실행 도구 모음과 설정 직렬화(M3) | 완료 |
| 백스테이지(M4) | 완료 |
| 키 팁/갤러리(M5) | 완료 |
| `find_package` 내보내기와 HCI/DPI 체크리스트(M6) | 완료 |
| 마무리와 실제 제품 통합 | [dev-plan](docs/en/dev-plan.md) 참조 |

## 라이선스

[MIT License](LICENSE)로 배포합니다.
