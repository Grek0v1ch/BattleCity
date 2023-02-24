cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Renderer/ShaderProgram.cpp
        src/Renderer/ShaderProgram.h
        src/ResourceManager/ResourceManager.cpp
        src/ResourceManager/ResourceManager.h
        src/ResourceManager/stb_image.h
        src/Renderer/Texture2D.cpp
        src/Renderer/Texture2D.h
        src/Renderer/Sprite.cpp
        src/Renderer/Sprite.h
        src/Renderer/AnimatedSprite.cpp
        src/Renderer/AnimatedSprite.h
        src/Exception/Exception.cpp
        src/Exception/Exception.h
        src/Game/Game.cpp
        src/Game/Game.h
        src/Game/Tank.cpp
        src/Game/Tank.h
        src/Renderer/VertexBuffer.cpp
        src/Renderer/VertexBuffer.h
        src/Renderer/IndexBuffer.cpp
        src/Renderer/IndexBuffer.h)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Обновить библиотеку: git subtree pull --prefix=external/glfw glfw master --squash
# Отключаем опции библиотеки GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

# указываем куда будем класть исполняемый файл
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# копируем папку res к исполняемому файлу
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)