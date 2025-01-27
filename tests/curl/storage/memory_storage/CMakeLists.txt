enable_testing()

add_executable(test_curl_memory_storage
    memory_storage.cpp
    MemoryStorage.cpp
    ../../../template/IntegrationTest.cpp
)
target_include_directories(test_curl_memory_storage
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_curl_memory_storage
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT CURL Download and Upload to Memory storage"
    COMMAND test_curl_memory_storage
)