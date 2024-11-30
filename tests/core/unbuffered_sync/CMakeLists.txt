enable_testing()

add_executable(test_unbuffered_sync
    unbuffered_sync.cpp
    UnbufferedSync.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_unbuffered_sync
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_unbuffered_sync
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Unbuffered Sync"
    COMMAND test_unbuffered_sync
)