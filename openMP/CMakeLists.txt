cmake_minimum_required(VERSION 2.8)

PROJECT(inverseMatrixOpenMP)

FIND_PACKAGE(OpenMP REQUIRED)
IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ENDIF()

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
target_link_libraries(${PROJECT_NAME} mpi)
