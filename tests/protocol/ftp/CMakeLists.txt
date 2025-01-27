add_subdirectory(performance)

enable_testing()

add_executable(test_ftp_upload_download_rel_paths
    upload_download_rel_paths.cpp
    UploadDownloadFile.cpp
    ServerAccess.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_ftp_upload_download_rel_paths
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_ftp_upload_download_rel_paths
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT FTP Upload and Download with relative file paths"
    COMMAND test_ftp_upload_download_rel_paths
)

add_executable(test_ftp_upload_download_abs_paths
    upload_download_abs_paths.cpp
    UploadDownloadFile.cpp
    ServerAccess.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_ftp_upload_download_abs_paths
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_ftp_upload_download_abs_paths
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT FTP Upload and Download with absolute file paths"
    COMMAND test_ftp_upload_download_abs_paths
)

add_executable(test_ftp_delete_remote_file
    delete_remote_file.cpp
    DeleteRemoteFile.cpp
    ServerAccess.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_ftp_delete_remote_file
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_ftp_delete_remote_file
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT FTP Delete Remote File"
    COMMAND test_ftp_delete_remote_file
)

add_executable(test_ftp_upload_download_dir
    upload_download_dir.cpp
    UploadDownloadDir.cpp
    ServerAccess.cpp
    ../../template/IntegrationTest.cpp
)
target_include_directories(test_ftp_upload_download_dir
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(test_ftp_upload_download_dir
    PRIVATE
        libfilesync
)
add_test(
    NAME "IT FTP Upload Download Directory"
    COMMAND test_ftp_upload_download_dir
)