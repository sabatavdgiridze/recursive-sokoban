cmake_minimum_required(VERSION 3.31)
project(recursive_sokoban)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/raylib/include)
link_directories(${CMAKE_SOURCE_DIR}/raylib/lib)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(recursive_sokoban main.cpp
        Chaikin.cpp
        Chaikin.h
        GameState/Board.cpp
        GameState/Board.h
        Animation.cpp
        Animation.h
        InputManager.cpp
        InputManager.h
        GameCamera.cpp
        GameCamera.h
        AnimationManager.cpp
        AnimationManager.h
        EventBus.cpp
        EventBus.h
        GameDataReader.cpp
        GameDataReader.h
        scenes/Scene.cpp
        scenes/Scene.h
        SierpinskiRenderer.cpp
        SierpinskiRenderer.h
        scenes/LevelScene.cpp
        scenes/LevelScene.h
        scenes/SceneManager.cpp
        scenes/SceneManager.h
        scenes/GameScene/GameScene.cpp
        scenes/GameScene/GameScene.h
        scenes/Operation.cpp
        scenes/Operation.h
        fonts/FontLoader.cpp
        fonts/FontLoader.h
        scenes/InitialScene.cpp
        scenes/InitialScene.h
        UI.cpp
        UI.h
        scenes/GameScene/CommandController.cpp
        scenes/GameScene/CommandController.h
        scenes/GameScene/Command.cpp
        scenes/GameScene/Command.h
        Buttons/UIButton.cpp
        Buttons/UIButton.h
        EdgeMerger.cpp
        EdgeMerger.h
        geometric_operations/Triangulation.cpp
        geometric_operations/Triangulation.h
        music/SoundtrackPlayer.cpp
        music/SoundtrackPlayer.h)

target_link_libraries(recursive_sokoban raylib opengl32 gdi32 winmm nlohmann_json::nlohmann_json)