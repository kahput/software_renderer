cmake_minimum_required(VERSION 4.0)
project(program)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(MSVC)
    set(CONFIG $<CONFIG>)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    set(CONFIG ${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CONFIG})
endif()

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.h" )
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "src/" ${CMAKE_BINARY_DIR})

if(LINUX)
    find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner)
    if (NOT WAYLAND_SCANNER_EXECUTABLE)
        message(FATAL_ERROR "Failed to find wayland-scanner")
    endif()

    macro(generate_wayland_protocol protocol_file)
        set(protocol_path "${CMAKE_SOURCE_DIR}/deps/wayland/${protocol_file}")

        string(REGEX REPLACE "\\.xml$" "-client-protocol.h" header_file ${protocol_file})
        string(REGEX REPLACE "\\.xml$" "-client-protocol-code.h" code_file ${protocol_file})

        add_custom_command(OUTPUT ${header_file}
            COMMAND "${WAYLAND_SCANNER_EXECUTABLE}" client-header "${protocol_path}" ${header_file}
            DEPENDS "${protocol_path}"
            VERBATIM)

        add_custom_command(OUTPUT ${code_file}
            COMMAND "${WAYLAND_SCANNER_EXECUTABLE}" private-code "${protocol_path}" ${code_file}
            DEPENDS "${protocol_path}"
            VERBATIM)

        target_sources(${PROJECT_NAME} PRIVATE ${header_file} ${code_file})
    endmacro()

    generate_wayland_protocol("wayland.xml")
    generate_wayland_protocol("viewporter.xml")
    generate_wayland_protocol("xdg-shell.xml")
    generate_wayland_protocol("fractional-scale-v1.xml")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WAYLAND PLATFORM_X11)
endif()

if(MSVC)
    # /WX
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # -Wpedantic
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable)
    target_link_libraries( ${PROJECT_NAME} dl)
endif()
