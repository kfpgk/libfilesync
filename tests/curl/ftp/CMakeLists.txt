enable_testing()

add_executable(test_curl_ftp_upload_download
    upload_download.cpp
    UploadDownload.cpp
    ../../template/IntegrationTest.cpp
)
target_link_libraries(test_curl_ftp_upload_download PRIVATE libfilesync)
add_test(
    NAME "IT CURL FTP Upload and Download"
    COMMAND test_curl_ftp_upload_download
)