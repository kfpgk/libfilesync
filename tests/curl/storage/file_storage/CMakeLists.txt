enable_testing()

add_executable(test_curl_file_storage
    file_storage.cpp
    FileStorage.cpp
    ../../../template/IntegrationTest.cpp
)
target_include_directories(test_curl_file_storage
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_curl_file_storage
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT CURL File Storage Basic Object operations"
    COMMAND test_curl_file_storage
)