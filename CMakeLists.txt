cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -Wshadow -W -Ofast")

project(out)

find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/main.cpp
                    ${PROJECT_SOURCE_DIR}/Camera.cpp
                    ${PROJECT_SOURCE_DIR}/Mesh.cpp
                    ${PROJECT_SOURCE_DIR}/Object.cpp
                    ${PROJECT_SOURCE_DIR}/obj_read.cpp
                    ${PROJECT_SOURCE_DIR}/Quaternion.cpp
                    ${PROJECT_SOURCE_DIR}/Render.cpp
                    ${PROJECT_SOURCE_DIR}/Scene.cpp
                    ${PROJECT_SOURCE_DIR}/Transform.cpp
                    ${PROJECT_SOURCE_DIR}/Vector2.cpp
                    ${PROJECT_SOURCE_DIR}/Vector3.cpp
                    ${PROJECT_SOURCE_DIR}/Window.cpp
                    ${PROJECT_SOURCE_DIR}/Light.cpp)
    
set(PROJECT_INCLUDE_DIR 
    ${PROJECT_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIR})

include_directories(${PROJECT_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${PNG_LIBRARY})

#sudo apt install libsdl2-dev libsdl2-image-dev -y