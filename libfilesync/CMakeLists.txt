add_library(libfilesync
    FileSync.cpp
    FileSyncException.cpp
    FileSyncLocks.cpp  
)

set_target_properties(libfilesync PROPERTIES PREFIX "")

set(PUBLIC_INCLUDE_DIR ${CMAKE_BINARY_DIR}/libfilesync/include)

configure_file(
    FileSync.hpp ${PUBLIC_INCLUDE_DIR}/libfilesync/FileSync.hpp COPYONLY
)
configure_file(
    FileSyncLocks.hpp ${PUBLIC_INCLUDE_DIR}/libfilesync/FileSyncLocks.hpp COPYONLY
)
configure_file(
    FileSyncException.hpp ${PUBLIC_INCLUDE_DIR}/libfilesync/FileSyncException.hpp COPYONLY
)

target_include_directories(libfilesync
    PUBLIC
        ${PUBLIC_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(libfilesync 
    PRIVATE
        curl
)
target_sources(libfilesync
    PUBLIC FILE_SET public_headers TYPE HEADERS
    FILES
        FileSync.hpp
        FileSyncException.hpp
        FileSyncLocks.hpp
)

add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(data)
add_subdirectory(protocol)
add_subdirectory(curl)
add_subdirectory(utility)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(FileSyncException_test
        FileSyncException.test.cpp
    )
    target_include_directories(FileSyncException_test 
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(FileSyncException_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT FileSyncException"
        COMMAND FileSyncException_test
    )
endif()