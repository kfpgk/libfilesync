enable_testing()

add_executable(test_ftp_upload_download_performance
    upload_download_performance.cpp
    UploadDownloadPerformance.cpp
    ../ServerAccess.cpp
    ../../../template/IntegrationTest.cpp
)
target_include_directories(test_ftp_upload_download_performance
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_ftp_upload_download_performance
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT FTP Upload and Download Performance Tests"
    COMMAND test_ftp_upload_download_performance
)