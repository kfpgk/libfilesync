enable_testing()

add_executable(test_local_first_resolve_conflict
    resolve_conflict.cpp
    ResolveConflict.cpp
    ../ResolveConflict.cpp
    ../../template/IntegrationTest.cpp
)
target_link_libraries(test_local_first_resolve_conflict PRIVATE libfilesync)
add_test(
    NAME "IT Local First Conflict Resolver"
    COMMAND test_local_first_resolve_conflict
)