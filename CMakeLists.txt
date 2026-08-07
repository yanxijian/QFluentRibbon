cmake_minimum_required(VERSION 3.21)

project(QFluentRibbon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
	add_compile_options(/MP)
endif()

option(QFR_BUILD_SHARED "Build qfr_ribbon as a shared library" ON)
option(QFR_BUILD_EXAMPLES "Build demo application (qfr_demo)" ON)
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
	add_library(qfr_ribbon SHARED ${QFR_LIB_SOURCES})
else()
	add_library(qfr_ribbon STATIC ${QFR_LIB_SOURCES})
endif()
add_library(QFluentRibbon::ribbon ALIAS qfr_ribbon)

set(QFR_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${QFR_GENERATED_INCLUDE_DIR}/qfluentribbon")
generate_export_header(qfr_ribbon
	BASE_NAME QFR
	EXPORT_MACRO_NAME QFR_EXPORT
	EXPORT_FILE_NAME "${QFR_GENERATED_INCLUDE_DIR}/qfluentribbon/qfluentribbon_export.hpp"
)

target_include_directories(qfr_ribbon
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${QFR_GENERATED_INCLUDE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(qfr_ribbon PUBLIC Qt6::Widgets)

set_target_properties(qfr_ribbon PROPERTIES
	OUTPUT_NAME qfr_ribbon
	EXPORT_NAME ribbon
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

if(QFR_BUILD_EXAMPLES)
	find_package(QThemeEngine CONFIG REQUIRED)
	add_executable(qfr_demo
		examples/gallery/main.cpp
		examples/gallery/qte_sync.cpp
		examples/gallery/qte_sync.hpp
	)
	target_link_libraries(qfr_demo PRIVATE qfr_ribbon QThemeEngine::engine)
	set_target_properties(qfr_demo PROPERTIES WIN32_EXECUTABLE TRUE)
	if(WIN32)
		find_program(QFR_WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${Qt6_DIR}/../../../bin" "$ENV{QTDIR}/bin")
		add_custom_command(TARGET qfr_demo POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
					$<TARGET_FILE:qfr_ribbon>
					$<TARGET_FILE_DIR:qfr_demo>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
					$<TARGET_FILE:QThemeEngine::engine>
					$<TARGET_FILE_DIR:qfr_demo>
			COMMENT "Copy qfr_ribbon.dll + qte_engine.dll beside qfr_demo"
			VERBATIM
		)
		if(QFR_WINDEPLOYQT_EXECUTABLE)
			add_custom_command(TARGET qfr_demo POST_BUILD
				COMMAND "${QFR_WINDEPLOYQT_EXECUTABLE}" --no-translations --compiler-runtime
					"$<TARGET_FILE:qfr_demo>"
				COMMENT "windeployqt: deploy Qt runtime beside qfr_demo"
			)
		endif()
	endif()
endif()

if(QFR_BUILD_TESTS)
	find_package(Qt6 REQUIRED COMPONENTS Test)
	enable_testing()
	add_executable(qfr_tests tests/test_collapse.cpp)
	target_link_libraries(qfr_tests PRIVATE qfr_ribbon Qt6::Test)
	add_test(NAME qfr_tests COMMAND qfr_tests)

	add_executable(qfr_qat_tests tests/test_qat_state.cpp)
	target_link_libraries(qfr_qat_tests PRIVATE qfr_ribbon Qt6::Test)
	add_test(NAME qfr_qat_tests COMMAND qfr_qat_tests)

	if(WIN32 AND QFR_BUILD_SHARED)
		foreach(_qfr_test IN ITEMS qfr_tests qfr_qat_tests)
			add_custom_command(TARGET ${_qfr_test} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
						$<TARGET_FILE:qfr_ribbon>
						$<TARGET_FILE_DIR:${_qfr_test}>
				COMMENT "Copy qfr_ribbon.dll beside ${_qfr_test}"
				VERBATIM
			)
		endforeach()
	endif()
endif()

if(QFR_INSTALL)
	include(CMakePackageConfigHelpers)

	install(TARGETS qfr_ribbon
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
