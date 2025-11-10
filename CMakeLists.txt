cmake_minimum_required(VERSION 3.10)
project(Casa3D)

# Define C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra as bibliotecas necessárias
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Inclui GLM (header-only)
find_package(glm REQUIRED)

# Arquivos fonte
set(SOURCES
    main.cpp
)

# Arquivos header
set(HEADERS
    Camera.h
    Shader.h
    Texture.h
    Mesh.h
    Model.h
    Geometry.h
)

# Cria o executável
add_executable(casa3d ${SOURCES} ${HEADERS})

# Link com as bibliotecas
target_link_libraries(casa3d
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Inclui GLM
target_include_directories(casa3d PRIVATE ${GLM_INCLUDE_DIRS})

# Copia shaders para o diretório de build
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Copia texturas se existirem
if(EXISTS ${CMAKE_SOURCE_DIR}/res)
    file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Avisos adicionais
if(MSVC)
    target_compile_options(casa3d PRIVATE /W4)
else()
    target_compile_options(casa3d PRIVATE -Wall -Wextra -Wpedantic)
endif()
