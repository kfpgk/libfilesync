#include <tests/protocol/ftp/DeleteRemoteFile.hpp>
#include <tests/config/config.h>

namespace test = filesync::integrationtest;

int main(int argc, char* argv[]) {

    test::protocol::ftp::DeleteRemoteFile test("Test FTP delete file",
        LIBFILESYNC_TEST_FTP_SERVER, LIBFILESYNC_TEST_FTP_SERVER_DIR);
    
    test.setUploadFileName("test_file_1");
    test.setRemoteFileName("to_be_deleted");
    
    test.setExitOnFail(true);
    test.run();

    return 0;
}