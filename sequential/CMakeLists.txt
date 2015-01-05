cmake_minimum_required(VERSION 2.8)

PROJECT(inverseMatrix)

SET(CMAKE_COLOR_MAKEFILE ON)

SET(SOURCE_DIR src)
SET(HEADERS_DIR include)

INCLUDE_DIRECTORIES(
                ${HEADERS_DIR}
)

SET(project_SOURCE
                ${SOURCE_DIR}/main.c
                ${SOURCE_DIR}/matrix.c
                ${SOURCE_DIR}/lu.c
)

SET(project_HEADERS
                ${HEADERS_DIR}/matrix.h
                ${HEADERS_DIR}/lu.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${project_HEADERS} ${project_SOURCE})
target_link_libraries(${PROJECT_NAME} m)
