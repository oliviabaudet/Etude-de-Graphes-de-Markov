cmake_minimum_required(VERSION 3.24)
project(TI_301_PRJ_STUDENTS-master C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PRJ_STUDENTS-master
        hasse.c
        hasse.h
        main.c
        utils.h
        utils.c
        matrix.c)


# Copier le dossier data dans le dossier de build
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
