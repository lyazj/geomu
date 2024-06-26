cmake_minimum_required(VERSION 3.19)
project(geomu VERSION 0.0.1)

# 设置项目包含目录、库目录和可执行文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# GENIE 编译选项
execute_process(COMMAND genie-config --libs OUTPUT_VARIABLE GENIE_LINK_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
separate_arguments(GENIE_LINK_LIBRARIES)
set(GENIE_LINK_LIBRARIES "-Wl,--start-group" ${GENIE_LINK_LIBRARIES} "-Wl,--end-group")  # 处理循环依赖
execute_process(COMMAND genie-config --topsrcdir OUTPUT_VARIABLE GENIE_SOURCE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
include_directories(SYSTEM ${GENIE_SOURCE_DIR})
execute_process(COMMAND genie-config --libdir OUTPUT_VARIABLE GENIE_LIBRARY_DIR OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
link_directories(${GENIE_LIBRARY_DIR})
execute_process(COMMAND readlink -f "${GENIE_SOURCE_DIR}/.." OUTPUT_VARIABLE GENIE OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
add_compile_definitions(GENIE="${GENIE}")

# ROOT 编译选项
find_package(ROOT)
set(ROOT_CC_FLAG_LIST ${ROOT_CC_FLAGS})
set(ROOT_CXX_FLAG_LIST ${ROOT_CXX_FLAGS})
separate_arguments(ROOT_CC_FLAG_LIST)
separate_arguments(ROOT_CXX_FLAG_LIST)
add_compile_options(${ROOT_CXX_FLAG_LIST})
add_compile_definitions(${ROOT_DEFINITIONS})
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})
set(ROOT_LIBRARIES ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

# 优化、调试和通用编译选项
option(ENABLE_SANITIZER "compile and link with sanitizing flags" OFF)
option(ENABLE_GPROF "compile and link with gprof flags" OFF)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif()
if(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    add_compile_options(-O0 -fno-omit-frame-pointer)
    if(ENABLE_SANITIZER)
        set(SANITIZER_OPTIONS -fno-sanitize-recover=all)
        list(APPEND SANITIZER_OPTIONS -fsanitize=address)
        list(APPEND SANITIZER_OPTIONS -fsanitize=leak)
        list(APPEND SANITIZER_OPTIONS -fsanitize=undefined)
        list(APPEND SANITIZER_OPTIONS -fsanitize=float-divide-by-zero)
        add_compile_options(${SANITIZER_OPTIONS})
        add_link_options(${SANITIZER_OPTIONS})
    endif()
else()
    add_compile_options(-O3)
endif()
if(ENABLE_GPROF)
    set(GPROF_OPTIONS -pg)
    add_compile_options(${GPROF_OPTIONS})
    add_link_options(${GPROF_OPTIONS})
endif()
add_compile_options(-Wall -Wshadow -Wextra)
add_compile_definitions(_GNU_SOURCE)
add_compile_definitions(PROJECT_BASEDIR="${CMAKE_SOURCE_DIR}")
add_compile_definitions(DIAG_SERIALIZE)

# 以下为静态库文件构建

file(GLOB GEOMU_SOURCES src/geomu/*.cpp)
add_library(geomu ${GEOMU_SOURCES})
target_link_libraries(geomu PUBLIC ${GENIE_LINK_LIBRARIES} EGPythia6 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} log4cpp xml2)

# 以下为可执行文件构建

add_executable(geomu-run src/geomu-run.cpp)
target_link_libraries(geomu-run PRIVATE geomu)

add_executable(ntuple-ntpmc src/ntuple-ntpmc.cpp)
target_link_libraries(ntuple-ntpmc PRIVATE geomu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/test)
file(GLOB TEST_SOURCES test/*.cpp)
foreach(TEST_SOURCE ${TEST_SOURCES})
    string(REPLACE ".cpp" "" TEST_BINARY ${TEST_SOURCE})
    string(REGEX REPLACE ".*/" "" TEST_BINARY ${TEST_BINARY})
    add_executable(${TEST_BINARY} ${TEST_SOURCE})
    target_link_libraries(${TEST_BINARY} PRIVATE geomu)
endforeach()
