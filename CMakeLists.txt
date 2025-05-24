cmake_minimum_required(VERSION 3.15)

set(PROJECT_VERSION_STR 1.2.0)

project(flash_tokenizer VERSION ${PROJECT_VERSION_STR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_EXECUTABLE "Build the executable" ON)


find_package(OpenMP)

set(SRC_FILES prj/src/bert_tokenizer.cpp)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

message(STATUS "Python Interpreter: ${Python3_EXECUTABLE}")
message(STATUS "Python Include Dir: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python Library: ${Python3_LIBRARIES}")
message(STATUS "Python Version: ${Python3_VERSION}")


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(COMPILE_OPTIONS -Ofast -funroll-loops -floop-nest-optimize
            -fstrict-overflow -fomit-frame-pointer -fipa-cp-clone)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-deprecated-declarations)
    add_definitions(-DDO_FORCE_INLINE)
    set(COMPILE_OPTIONS -O3 -flto -funroll-loops -ffast-math
            -fomit-frame-pointer -fvectorize -fslp-vectorize -fno-finite-math-only
            -fstrict-aliasing -finline-functions
            -march=native -mtune=native)
elseif (MSVC)
    set(COMPILE_OPTIONS /O2 /W1 /GL /Ot /Ob3 /fp:fast /arch:AVX2 /Zc:__cplusplus /EHsc- /GR- /link /STACK:4194304)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()


if (BUILD_EXECUTABLE)
    add_executable(flash_tokenizer main.cpp ${SRC_FILES})
    target_include_directories(flash_tokenizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/prj/include)


    target_include_directories(flash_tokenizer PRIVATE ${Python3_INCLUDE_DIRS})
    target_link_libraries(flash_tokenizer PRIVATE ${Python3_LIBRARIES})

    if (OpenMP_CXX_FOUND)
        target_link_libraries(flash_tokenizer PRIVATE OpenMP::OpenMP_CXX)
    endif ()

    target_compile_options(flash_tokenizer PRIVATE ${COMPILE_OPTIONS})

    add_custom_command(TARGET flash_tokenizer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/data_loader.py
            $<TARGET_FILE_DIR:flash_tokenizer>/data_loader.py
    )
endif ()
