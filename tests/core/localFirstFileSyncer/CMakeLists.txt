enable_testing()

add_executable(test_local_first_resolve_conflict
    resolve_conflict.cpp
    ResolveConflict.cpp
    ../ResolveConflict.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_local_first_resolve_conflict
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_local_first_resolve_conflict 
    PRIVATE 
        libfilesync
)
add_test(
    NAME "IT Local First Conflict Resolver"
    COMMAND test_local_first_resolve_conflict
)