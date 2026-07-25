cmake_minimum_required(VERSION 3.21)
project(QFluentRibbon VERSION 0.1.0 LANGUAGES CXX)

# Scaffold only — implementation starts at M0 (see docs/zh/dev-plan.md).
# Planned: Qt6 Widgets + find_package(QThemeEngine) / path override.

option(QFR_BUILD_EXAMPLES "Build gallery example" ON)
option(QFR_BUILD_TESTS "Build tests" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "QFluentRibbon ${PROJECT_VERSION}: documentation-first scaffold (no library targets yet).")
message(STATUS "  Docs: docs/zh/architecture.md , docs/zh/dev-plan.md")

# Placeholder so the tree is reserved for M0.
add_custom_target(qfluentribbon_docs SOURCES
  README.md
  docs/zh/architecture.md
  docs/zh/dev-plan.md
  docs/en/architecture.md
  docs/en/dev-plan.md
)
