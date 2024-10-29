enable_testing()

add_executable(test_create_recursive_entries
    create_recursive_entries.cpp
    CreateRecursiveEntries.cpp
    ../template/IntegrationTest.cpp
)
target_include_directories(test_create_recursive_entries
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_create_recursive_entries
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Create entries recursively"
    COMMAND test_create_recursive_entries
)

add_executable(test_file_changed
    file_changed.cpp
    FileChanged.cpp
    ../template/IntegrationTest.cpp
)
target_include_directories(test_file_changed
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_file_changed
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT Detection of File Changed"
    COMMAND test_file_changed
)