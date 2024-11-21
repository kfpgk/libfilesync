target_sources(libfilesync
    PRIVATE
        Storage.cpp
        FileStorage.cpp
        MemoryStorage.cpp
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
endif()