add_library(libfilesync
    FileSyncException.cpp
    FileSync.cpp
)

target_include_directories(libfilesync 
    PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(libfilesync 
    PRIVATE
        curl
)

add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(data)
add_subdirectory(protocol)
add_subdirectory(curl)
add_subdirectory(utility)

if(BUILD_TESTING)
    enable_testing()
    add_executable(FileSyncException_test
        FileSyncException.test.cpp
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