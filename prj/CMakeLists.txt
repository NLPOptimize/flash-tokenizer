cmake_minimum_required(VERSION 3.15)

set(PROJECT_VERSION_STR 1.3.0)

project(flash_tokenizer VERSION ${PROJECT_VERSION_STR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_PYTHON_MODULE "Build the Python module" ON)


find_package(OpenMP)

set(SRC_FILES src/bert_tokenizer.cpp)

#add_definitions(-DUSE_ROBIN_HOOD_LIB)
add_definitions(-DUSE_ANKERL_LIB)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(COMPILE_OPTIONS -Ofast -funroll-loops -floop-nest-optimize
            -fstrict-overflow -fomit-frame-pointer
            -fipa-cp-clone)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-deprecated-declarations)
    add_definitions(-DDO_FORCE_INLINE)
    set(COMPILE_OPTIONS -O3 -flto -funroll-loops -ffast-math
            -fomit-frame-pointer -fvectorize -fslp-vectorize -fno-finite-math-only
            -fstrict-aliasing -finline-functions)
elseif (MSVC)
    set(COMPILE_OPTIONS /O2 /W1 /GL /Ot /fp:fast /arch:AVX2 /Zc:__cplusplus /GR-)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()


if (BUILD_PYTHON_MODULE)
    find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

    execute_process(
            COMMAND "${Python_EXECUTABLE}" -c "import pybind11; print(pybind11.get_cmake_dir())"
            OUTPUT_VARIABLE _pybind11_cmake_dir
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    list(APPEND CMAKE_PREFIX_PATH "${_pybind11_cmake_dir}")
    find_package(pybind11 REQUIRED)


    add_library(bert_tokenizer STATIC
            ${SRC_FILES}
    )
    target_include_directories(bert_tokenizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    pybind11_add_module(_core python/src/bindings.cpp)
    target_link_libraries(_core PRIVATE bert_tokenizer)

    if (OpenMP_CXX_FOUND)
        target_link_libraries(_core PRIVATE OpenMP::OpenMP_CXX)
    endif ()

    target_compile_options(_core PRIVATE ${COMPILE_OPTIONS})
    target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})

    install(TARGETS _core DESTINATION flash_tokenizer)
endif ()