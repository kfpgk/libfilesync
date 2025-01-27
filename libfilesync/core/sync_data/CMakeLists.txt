add_subdirectory(buffer)

target_sources(libfilesync 
    PRIVATE
        Entry.cpp
        File.cpp
        Directory.cpp
        EntryFactory.cpp
        RemoteEntry.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(SyncDirectory_test
        Directory.test.cpp
    )
    target_include_directories(SyncDirectory_test 
        PRIVATE
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(SyncDirectory_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT core::sync_data::Directory"
        COMMAND SyncDirectory_test
    )
    
    add_executable(SyncEntry_test
        Entry.test.cpp
    )
    target_include_directories(SyncEntry_test
        PRIVATE
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(SyncEntry_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT core::sync_data::Entry"
        COMMAND SyncEntry_test
    )

    add_executable(RemoteEntry_test
        RemoteEntry.test.cpp
    )
    target_include_directories(RemoteEntry_test
        PRIVATE
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(RemoteEntry_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT core::sync_data::RemoteEntry"
        COMMAND RemoteEntry_test
    )
endif()