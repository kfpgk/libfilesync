target_sources(libfilesync 
    PRIVATE
        Data.cpp
        DataException.cpp
        Directory.cpp
        Entry.cpp
        File.cpp
        RemoteEntry.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(Directory_test
        Directory.test.cpp
    )
    target_link_libraries(Directory_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT Directory"
        COMMAND Directory_test
    )
endif()