add_subdirectory(option)
add_subdirectory(parser)
add_subdirectory(storage)
add_subdirectory(wrapper)

target_sources(libfilesync 
    PRIVATE
        Exception.cpp
        ProtocolClient.cpp
        FtpClient.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(CurlProtocolClient_test
        wrapper/EasyStub.test.cpp
        parser/NobodyStub.test.cpp
        ProtocolClient.test.cpp
    )
    target_include_directories(CurlProtocolClient_test
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(CurlProtocolClient_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT curl::ProtocolClient"
        COMMAND CurlProtocolClient_test
    )
endif()