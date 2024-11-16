enable_testing()

add_executable(test_sync_data_file_buffer
    file_buffer.cpp
    FileBuffer.cpp
    ../../../template/IntegrationTest.cpp    
)
target_include_directories(test_sync_data_file_buffer
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_sync_data_file_buffer
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Sync Data File Buffer"
    COMMAND test_sync_data_file_buffer
)