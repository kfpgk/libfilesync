target_sources(libfilesync 
    PRIVATE
        Buffer.cpp
        Directory.cpp
        Entry.cpp
        EntryFactory.cpp
        File.cpp
        RemoteEntry.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(SyncDirectory_test
        Directory.test.cpp
    )
    target_link_libraries(SyncDirectory_test
        PRIVATE
            libfilesync
    )
    target_include_directories(SyncDirectory_test 
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    add_test(
        NAME "UT SyncDirectory"
        COMMAND SyncDirectory_test
    )
endif()