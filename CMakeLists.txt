cmake_minimum_required(VERSION 3.21)

project(QFluentRibbon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

option(QFR_BUILD_EXAMPLES "Build gallery example" ON)
option(QFR_BUILD_TESTS "Build tests" OFF)

# QTE: sibling path / -DQFR_QTE_SOURCE_DIR=... / find_package(QThemeEngine)
set(QFR_QTE_SOURCE_DIR "" CACHE PATH "Path to QThemeEngine sources (optional; default ../QThemeEngine)")
if(QFR_QTE_SOURCE_DIR STREQUAL "" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../QThemeEngine/CMakeLists.txt")
	set(QFR_QTE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../QThemeEngine")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(_qfr_have_qte_target FALSE)
if(TARGET QThemeEngine::engine OR TARGET qtheme_engine)
	set(_qfr_have_qte_target TRUE)
elseif(QFR_QTE_SOURCE_DIR AND EXISTS "${QFR_QTE_SOURCE_DIR}/CMakeLists.txt")
	set(QTE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(QTE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(QTE_BUILD_WIDGETS OFF CACHE BOOL "" FORCE)
	set(QTE_INSTALL OFF CACHE BOOL "" FORCE)
	add_subdirectory("${QFR_QTE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_deps/qthemeengine" EXCLUDE_FROM_ALL)
	set(_qfr_have_qte_target TRUE)
	message(STATUS "QFluentRibbon: using QThemeEngine from ${QFR_QTE_SOURCE_DIR}")
else()
	find_package(QThemeEngine CONFIG QUIET)
	if(TARGET QThemeEngine::engine)
		set(_qfr_have_qte_target TRUE)
		message(STATUS "QFluentRibbon: using installed QThemeEngine::engine")
	endif()
endif()

if(NOT _qfr_have_qte_target)
	message(FATAL_ERROR
		"QThemeEngine not found. Options:\n"
		"  - Place QThemeEngine next to this repo (../QThemeEngine)\n"
		"  - Pass -DQFR_QTE_SOURCE_DIR=/path/to/QThemeEngine\n"
		"  - Install QTE and pass -DCMAKE_PREFIX_PATH=...")
endif()

if(TARGET QThemeEngine::engine)
	set(QFR_QTE_TARGET QThemeEngine::engine)
else()
	set(QFR_QTE_TARGET qtheme_engine)
endif()

add_library(qfluentribbon
	src/theme_bridge.cpp
	src/ribbon_bar.cpp
	src/ribbon_window.cpp
	include/qfluentribbon/theme_bridge.hpp
	include/qfluentribbon/ribbon_bar.hpp
	include/qfluentribbon/ribbon_window.hpp
	include/qfluentribbon/qfluentribbon.hpp
)
add_library(QFluentRibbon::qfluentribbon ALIAS qfluentribbon)

target_include_directories(qfluentribbon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(qfluentribbon PUBLIC Qt6::Widgets ${QFR_QTE_TARGET})

set_target_properties(qfluentribbon PROPERTIES
	OUTPUT_NAME qfluentribbon
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(QFR_BUILD_EXAMPLES)
	add_executable(qfr_gallery examples/gallery/main.cpp)
	target_link_libraries(qfr_gallery PRIVATE qfluentribbon)
	set_target_properties(qfr_gallery PROPERTIES WIN32_EXECUTABLE TRUE)
	if(WIN32)
		find_program(QFR_WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin")
		if(QFR_WINDEPLOYQT_EXECUTABLE)
			add_custom_command(TARGET qfr_gallery POST_BUILD
				COMMAND "${QFR_WINDEPLOYQT_EXECUTABLE}" --no-translations --compiler-runtime
					"$<TARGET_FILE:qfr_gallery>"
				COMMENT "windeployqt: deploy Qt runtime beside qfr_gallery"
			)
		endif()
	endif()
endif()

message(STATUS "QFluentRibbon ${PROJECT_VERSION}: library + examples=${QFR_BUILD_EXAMPLES}")
