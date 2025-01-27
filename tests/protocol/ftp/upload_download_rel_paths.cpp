#include <tests/protocol/ftp/UploadDownloadFile.hpp>
#include <tests/config/config.h>

namespace test = filesync::integration_test;

int main(int argc, char* argv[]) {

    test::protocol::ftp::UploadDownloadFile test("Test FTP upload and download with relative paths",
        LIBFILESYNC_TEST_FTP_SERVER, LIBFILESYNC_TEST_FTP_SERVER_DIR);
    
    test.setUploadFileName("test_file_1");
    test.setDownloadFileName("test_file_2");
    
    test.run();

    return 0;

}