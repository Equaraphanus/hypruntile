cmake_minimum_required(VERSION 3.27)

project(hypruntile
    DESCRIPTION "A plugin to remove XDG_TOPLEVEL_STATE_TILED_* flags from certain windows."
    VERSION 0.1
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC "*.cpp")

add_library(hypruntile SHARED ${SRC})

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET
    hyprland
    libdrm
    libinput
    libudev
    pangocairo
    pixman-1
    wayland-server
    xkbcommon
)
target_link_libraries(hypruntile PRIVATE rt PkgConfig::deps)

install(TARGETS hypruntile)
