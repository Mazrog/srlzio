cmake_minimum_required(VERSION 3.14)
project(srlzio)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(${PROJECT_NAME}
        srlzio/srlzio.hpp

        sample/models.hpp
        sample/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2)

add_executable(${PROJECT_NAME}_tests
        srlzio/srlzio.hpp

        tests/models.hpp
        tests/main.tests.cpp
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE tinyxml2)