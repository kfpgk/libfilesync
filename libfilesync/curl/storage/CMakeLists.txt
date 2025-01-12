add_subdirectory(char_buffer)
add_subdirectory(visitor)

target_sources(libfilesync
    PRIVATE   
        FileStorage.cpp
        MemoryStorage.cpp
        MemoryStorageHandle.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(FileStorage_test
        FileStorage.test.cpp
    )
    target_include_directories(FileStorage_test
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(FileStorage_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT curl::storage::FileStorage"
        COMMAND FileStorage_test
    )

    add_executable(MemoryStorageHandle_test
        MemoryStorageHandle.test.cpp
    )
    target_include_directories(MemoryStorageHandle_test
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(MemoryStorageHandle_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT curl::storage::MemoryStorageHandle"
        COMMAND MemoryStorageHandle_test
    )
endif()