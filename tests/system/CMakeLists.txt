enable_testing()
add_executable(test_file_sync
    file_sync.cpp
    FileSync.cpp
    ../template/IntegrationTest.cpp
)
target_link_libraries(test_file_sync PRIVATE libfilesync)
add_test(
    NAME "IT File Sync"
    COMMAND test_file_sync
)