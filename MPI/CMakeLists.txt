cmake_minimum_required(VERSION 2.8)

PROJECT(inverseMatrixMPI)

FIND_PACKAGE(MPI REQUIRED)
IF(MPI_FOUND)
    SET(CMAKE_C_COMPILER mpicc)
    SET(CMAKE_CXX_COMPILER mpicxx)
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
                ${SOURCE_DIR}/utils.c
)

SET(project_HEADERS
                ${HEADERS_DIR}/matrix.h
                ${HEADERS_DIR}/lu.h
                ${HEADERS_DIR}/utils.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${project_HEADERS} ${project_SOURCE})
target_link_libraries(${PROJECT_NAME} m mpi)
