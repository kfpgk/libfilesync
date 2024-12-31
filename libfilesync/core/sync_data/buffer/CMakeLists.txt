add_subdirectory(visitor)

target_sources(libfilesync
    PRIVATE
        CharArrayMemoryBuffer.cpp
        FileBuffer.cpp
        ProtocolMemoryBuffer.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(SyncDataMemoryBuffer_test
        ProtocolMemoryBuffer.test.cpp
    )
    target_include_directories(SyncDataMemoryBuffer_test 
        PRIVATE
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(SyncDataMemoryBuffer_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT core::sync_data::buffer::ProtocolMemoryBuffer"
        COMMAND SyncDataMemoryBuffer_test
    )
    
endif()