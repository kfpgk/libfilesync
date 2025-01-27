enable_testing()

add_executable(test_buffered_sync
    buffered_sync.cpp
    BufferedSync.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_buffered_sync
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_buffered_sync
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Buffered Sync"
    COMMAND test_buffered_sync
)