cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "-fopenmp")

include(FetchContent)

find_package(OpenMP REQUIRED)

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

set(GAME_LOGIC_SOURCES
        Sources/SequentialGame.cpp
        Sources/OpenMPGame.cpp
        Sources/AlgorithmFactory.cpp
        Sources/CommandImpl/PlayCommand.cpp
        Sources/CommandImpl/PauseCommand.cpp
        Sources/CommandImpl/ClearCommand.cpp
        Sources/CommandImpl/SaveCommand.cpp
        Sources/CommandImpl/LoadCommand.cpp
)

set(GUI_SOURCES
        Sources/Game.cpp
        Sources/GUIPanel.cpp
        Sources/GameRenderer.cpp
)

set(BENCHMARK_SOURCES
        Sources/Benchmark.cpp
)

# Application 1, with GUI
add_executable(
        Conway
        main_gui.cpp
        ${GUI_SOURCES}
        ${GAME_LOGIC_SOURCES}
)

target_link_libraries(Conway PRIVATE
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
        OpenMP::OpenMP_CXX
)


# Application 2, for benchmarking
add_executable(
        Benchmark
        main_benchmark.cpp
        ${BENCHMARK_SOURCES}
        ${GAME_LOGIC_SOURCES}
)

target_link_libraries(Benchmark PRIVATE
        OpenMP::OpenMP_CXX
)

target_include_directories(Conway PRIVATE ${CMAKE_SOURCE_DIR}/Headers)
target_include_directories(Benchmark PRIVATE ${CMAKE_SOURCE_DIR}/Headers)
target_compile_features(Conway PRIVATE cxx_std_20)
target_compile_features(Benchmark PRIVATE cxx_std_20)