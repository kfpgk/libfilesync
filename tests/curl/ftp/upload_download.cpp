#include <tests/curl/ftp/UploadDownload.hpp>
#include <tests/config/config.h>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::curl::ftp::UploadDownload test("curl ftp upload and download",
        LIBFILESYNC_TEST_FTP_SERVER,
        LIBFILESYNC_TEST_FTP_SERVER_DIR);

    test.setExitOnFail(true);
    test.run();

    return 0;
}