enable_testing()

add_executable(test_remote_first_resolve_conflict
    resolve_conflict.cpp
    ResolveConflict.cpp
    ../ResolveConflict.cpp
    ../../template/IntegrationTest.cpp
)
target_link_libraries(test_remote_first_resolve_conflict PRIVATE libfilesync)
add_test(
    NAME "IT Remote First Conflict Resolver"
    COMMAND test_remote_first_resolve_conflict
)