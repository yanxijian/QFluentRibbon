cmake_minimum_required(VERSION 3.21)

project(QFluentRibbon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

option(QFR_BUILD_SHARED "Build qfluentribbon as a shared library" ON)
option(QFR_BUILD_EXAMPLES "Build gallery example" ON)
option(QFR_BUILD_TESTS "Build tests" OFF)
option(QFR_INSTALL "Install library, headers, and CMake package" ON)

include(GNUInstallDirs)
include(GenerateExportHeader)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(QFR_LIB_SOURCES
	src/ribbon_tokens.cpp
	src/theme_bridge.cpp
	src/ribbon_bar.cpp
	src/ribbon_tab.cpp
	src/ribbon_group.cpp
	src/ribbon_window.cpp
	src/screen_tip.cpp
	src/quick_access_bar.cpp
	src/backstage.cpp
	src/key_tip.cpp
	src/ribbon_gallery.cpp
	src/layout/collapse.cpp
	include/qfluentribbon/ribbon_tokens.hpp
	include/qfluentribbon/theme_bridge.hpp
	include/qfluentribbon/ribbon_bar.hpp
	include/qfluentribbon/ribbon_tab.hpp
	include/qfluentribbon/ribbon_group.hpp
	include/qfluentribbon/ribbon_window.hpp
	include/qfluentribbon/ribbon_types.hpp
	include/qfluentribbon/screen_tip.hpp
	include/qfluentribbon/quick_access_bar.hpp
	include/qfluentribbon/backstage.hpp
	include/qfluentribbon/key_tip.hpp
	include/qfluentribbon/ribbon_gallery.hpp
	include/qfluentribbon/layout/collapse.hpp
	include/qfluentribbon/qfluentribbon.hpp
)

if(QFR_BUILD_SHARED)
	add_library(qfluentribbon SHARED ${QFR_LIB_SOURCES})
else()
	add_library(qfluentribbon STATIC ${QFR_LIB_SOURCES})
endif()
add_library(QFluentRibbon::qfluentribbon ALIAS qfluentribbon)

set(QFR_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${QFR_GENERATED_INCLUDE_DIR}/qfluentribbon")
generate_export_header(qfluentribbon
	BASE_NAME QFR
	EXPORT_MACRO_NAME QFR_EXPORT
	EXPORT_FILE_NAME "${QFR_GENERATED_INCLUDE_DIR}/qfluentribbon/qfluentribbon_export.hpp"
)

target_include_directories(qfluentribbon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${QFR_GENERATED_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(qfluentribbon PUBLIC Qt6::Widgets)

set_target_properties(qfluentribbon PROPERTIES
	OUTPUT_NAME qfluentribbon
	EXPORT_NAME qfluentribbon
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

if(QFR_BUILD_EXAMPLES)
	find_package(QThemeEngine CONFIG REQUIRED)
	add_executable(qfr_gallery
		examples/gallery/main.cpp
		examples/gallery/qte_sync.cpp
		examples/gallery/qte_sync.hpp
	)
	target_link_libraries(qfr_gallery PRIVATE qfluentribbon QThemeEngine::engine)
	set_target_properties(qfr_gallery PROPERTIES WIN32_EXECUTABLE TRUE)
	if(WIN32)
		find_program(QFR_WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin" "$ENV{QTDIR}/bin")
		add_custom_command(TARGET qfr_gallery POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
					$<TARGET_FILE:qfluentribbon>
					$<TARGET_FILE_DIR:qfr_gallery>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
					$<TARGET_FILE:QThemeEngine::engine>
					$<TARGET_FILE_DIR:qfr_gallery>
			COMMENT "Copy qfluentribbon.dll + qtheme_engine.dll beside qfr_gallery"
			VERBATIM
		)
		if(QFR_WINDEPLOYQT_EXECUTABLE)
			add_custom_command(TARGET qfr_gallery POST_BUILD
				COMMAND "${QFR_WINDEPLOYQT_EXECUTABLE}" --no-translations --compiler-runtime
					"$<TARGET_FILE:qfr_gallery>"
				COMMENT "windeployqt: deploy Qt runtime beside qfr_gallery"
			)
		endif()
	endif()
endif()

if(QFR_BUILD_TESTS)
	find_package(Qt6 REQUIRED COMPONENTS Test)
	enable_testing()
	add_executable(qfr_tests tests/test_collapse.cpp)
	target_link_libraries(qfr_tests PRIVATE qfluentribbon Qt6::Test)
	add_test(NAME qfr_tests COMMAND qfr_tests)

	add_executable(qfr_qat_tests tests/test_qat_state.cpp)
	target_link_libraries(qfr_qat_tests PRIVATE qfluentribbon Qt6::Test)
	add_test(NAME qfr_qat_tests COMMAND qfr_qat_tests)
endif()

if(QFR_INSTALL)
	include(CMakePackageConfigHelpers)

	install(TARGETS qfluentribbon
		EXPORT QFluentRibbonTargets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

	install(DIRECTORY include/qfluentribbon
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.hpp"
	)
	install(FILES "${QFR_GENERATED_INCLUDE_DIR}/qfluentribbon/qfluentribbon_export.hpp"
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/qfluentribbon
	)

	install(EXPORT QFluentRibbonTargets
		FILE QFluentRibbonTargets.cmake
		NAMESPACE QFluentRibbon::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QFluentRibbon
	)

	configure_package_config_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/cmake/QFluentRibbonConfig.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/QFluentRibbonConfig.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QFluentRibbon
	)

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/QFluentRibbonConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/QFluentRibbonConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/QFluentRibbonConfigVersion.cmake"
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QFluentRibbon
	)
endif()

message(STATUS "QFluentRibbon ${PROJECT_VERSION}: shared=${QFR_BUILD_SHARED} examples=${QFR_BUILD_EXAMPLES} tests=${QFR_BUILD_TESTS} install=${QFR_INSTALL}")
