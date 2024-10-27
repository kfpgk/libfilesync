#include <tests/protocol/ftp/UploadDownloadDir.hpp>
#include <tests/config/config.h>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::protocol::ftp::UploadDownloadDir test(
        "Test FTP upload and download directory",
        LIBFILESYNC_TEST_FTP_SERVER, 
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.setExitOnFail(true);
    test.run();

    return 0;
}