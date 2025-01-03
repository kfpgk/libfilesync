enable_testing()
add_executable(test_file_sync_error_handling
    file_sync_error_handling.cpp
    FileSyncErrorHandling.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_file_sync_error_handling
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_file_sync_error_handling
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT File Sync Error Handling"
    COMMAND test_file_sync_error_handling
)