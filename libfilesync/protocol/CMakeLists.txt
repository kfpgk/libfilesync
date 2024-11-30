target_sources(libfilesync 
    PRIVATE
        Client.cpp
        FtpClient.cpp
)

if(BUILD_UNIT_TESTS)
    enable_testing()
    add_executable(ProtocolClient_test
        Client.test.cpp
    )
    target_include_directories(ProtocolClient_test 
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_BINARY_DIR}
    )
    target_link_libraries(ProtocolClient_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT protocol::Client"
        COMMAND ProtocolClient_test
    )
endif()