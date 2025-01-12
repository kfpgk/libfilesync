add_subdirectory(visitor)

target_sources(libfilesync
    PRIVATE
        ReadBuffer.cpp
        ReadWriteBuffer.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()

    add_executable(ReadBuffer_test
        ReadBuffer.test.cpp
    )
    target_include_directories(ReadBuffer_test
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(ReadBuffer_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT curl::storage::char_buffer::ReadBuffer"
        COMMAND ReadBuffer_test
    )

    add_executable(ReadWriteBuffer_test
        ReadWriteBuffer.test.cpp
    )
    target_include_directories(ReadWriteBuffer_test
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(ReadWriteBuffer_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT curl::storage::char_buffer::ReadWriteBuffer"
        COMMAND ReadWriteBuffer_test
    )
endif()