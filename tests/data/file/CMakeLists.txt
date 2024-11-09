enable_testing()

add_executable(test_changed
    changed_main.cpp
    Changed.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_changed
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_changed
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Detection of File Changed"
    COMMAND test_changed
)