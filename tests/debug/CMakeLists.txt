enable_testing()

add_executable(test_debug
    debug_main.cpp
    Debug.cpp
    ../template/IntegrationTest.cpp
)
target_link_libraries(test_debug PRIVATE libfilesync)
add_test(
    NAME "IT Debug experimental test setup"
    COMMAND test_debug
)