target_sources(libfilesync 
    PRIVATE
        Data.cpp
        Directory.cpp
        Entry.cpp
        EntryFactory.cpp
        Exception.cpp
        File.cpp
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
    target_include_directories(Directory_test 
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    add_test(
        NAME "UT data::Directory"
        COMMAND Directory_test
    )
endif()