cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
        ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(ImGui)

FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)

FetchContent_Declare(
        ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml
        GIT_TAG v3.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable(
        Conway
        main.cpp
        Sources/SequentialGame.cpp
        Headers/ICommand.h
        Sources/CommandImpl/PlayCommand.cpp
        Headers/CommandImpl/PlayCommand.h
        Sources/CommandImpl/PauseCommand.cpp
        Headers/CommandImpl/PauseCommand.h
        Sources/CommandImpl/ClearCommand.cpp
        Headers/CommandImpl/ClearCommand.h
        Sources/CommandImpl/SaveCommand.cpp
        Headers/CommandImpl/SaveCommand.h
        Sources/CommandImpl/LoadCommand.cpp
        Headers/CommandImpl/LoadCommand.h
        Headers/GUIPanel.h
        Sources/GUIPanel.cpp
        Sources/GameRenderer.cpp
        Headers/GameRenderer.h
        Headers/IAppControls.h
        Headers/Game.h
        Sources/Game.cpp
)

target_include_directories(Conway PRIVATE ${CMAKE_SOURCE_DIR}/Headers)
target_compile_features(Conway PRIVATE cxx_std_20)
target_link_libraries(Conway PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)