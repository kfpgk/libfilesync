target_sources(libfilesync 
    PRIVATE
        Base.cpp
        Exception.cpp
        Url.cpp
        SList.cpp
        Setup.cpp
        ProtocolClient.cpp
        FtpClient.cpp
)

add_subdirectory(option)
add_subdirectory(interface)

if(BUILD_TESTING)
    enable_testing()
    add_executable(ProtocolClient_test
        interface/EasyStub.test.cpp
        ProtocolClient.test.cpp
    )
    target_link_libraries(ProtocolClient_test
        PRIVATE
            libfilesync
    )
    add_test(
        NAME "UT CurlProtocolClient"
        COMMAND ProtocolClient_test
    )
endif()