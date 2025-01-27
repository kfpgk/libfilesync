enable_testing()

add_executable(test_one_way_sync
    one_way_sync.cpp
    OneWaySync.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_one_way_sync
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
)
target_link_libraries(test_one_way_sync
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT One Way Sync"
    COMMAND test_one_way_sync
)