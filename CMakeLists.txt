cmake_minimum_required(VERSION 3.21.0)
project(umba-2c VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(UMBA_STATIC_RUNTIME ON)
set(UMBA_USE_BOOST      OFF)

include(${CMAKE_CURRENT_LIST_DIR}/.umba.cmake/umba.cmake)


include_directories(${LIB_ROOT})

# Import libraries here
#add_subdirectory(${LIB_ROOT}/2c_common)
add_subdirectory(${LIB_ROOT}/encoding)
add_subdirectory(${LIB_ROOT}/marty_cpp)
#add_subdirectory(${LIB_ROOT}/marty_utf)
add_subdirectory(${LIB_ROOT}/sfmt)
add_subdirectory(${LIB_ROOT}/umba)

# umba_configure_boost("STATIC_LIBS" "MULTITHREADED" "${STATIC_RUNTIME}")

# add_compile_definitions("YAML_CPP_STATIC_DEFINE")
# add_compile_definitions("USE_UMBA_LOUT_COUT")
# add_compile_definitions("UMBA_TOKENIZER_DISABLE_UMBA_TOKENIZER_GET_CHAR_CLASS_FUNCTION")
# add_compile_definitions("UMBA_TOKEN_TRIE_FIND_NEXT_BINARY_SEARCH_DISABLED")


# Configure libraries here
set(COMMON_LIBS 
    encoding::encoding
    marty::cpp
    sfmt::sfmt
    umba::umba
   )


file(GLOB_RECURSE sources   "${SRC_ROOT}/*.cpp")
file(GLOB_RECURSE headers   "${SRC_ROOT}/*.h")
file(GLOB_RECURSE resources "${SRC_ROOT}/*.rc")
umba_make_sources_tree("${SRC_ROOT}" "${sources}" "${headers}" "${resources}")


set(PLATFORM_LIBS)
if(WIN32) # Всё нормально, это про таргет
    list(APPEND PLATFORM_LIBS ws2_32 iphlpapi shell32)
endif()


set(COMMON_UMBA_OPTIONS "BIGOBJ" "UTF8" "PEDANTIC" "WERR") # "PEDANTIC"/"WALL" "WERR"


set(TARGET umba-2c)
# add_executable(${TARGET} "${SRC_ROOT}/${TARGET}.cpp" "${sources}" "${headers}")
add_executable(${TARGET} "${sources}" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")

