#include <tests/protocol/ftp/performance/UploadDownloadPerformance.hpp>
#include <tests/config/config.h>

int main(int argc, char* argv[]) {

    using namespace filesync::integration_test;
    protocol::ftp::performance::UploadDownloadPerformance test(
        "Test FTP upload download performance",
        LIBFILESYNC_TEST_FTP_SERVER, LIBFILESYNC_TEST_FTP_SERVER_DIR);
    
    test.run();

    return 0;
}