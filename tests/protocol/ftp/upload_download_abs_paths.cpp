#include <tests/protocol/ftp/UploadDownloadFile.hpp>
#include <tests/config/config.h>

#include <iostream>
#include <filesystem>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::protocol::ftp::UploadDownloadFile test("Test FTP upload and download with absolute paths",
        LIBFILESYNC_TEST_FTP_SERVER, LIBFILESYNC_TEST_FTP_SERVER_DIR);
    
    std::string absoluteUploadFilePath(std::filesystem::absolute("test_file_1"));
    std::string absoluteDownloadFilePath(std::filesystem::absolute("test_file_2"));

    test.setUploadFileName(absoluteUploadFilePath);
    test.setDownloadFileName(absoluteDownloadFilePath);
    
    test.setExitOnFail(true);
    test.run();

    return 0;

}