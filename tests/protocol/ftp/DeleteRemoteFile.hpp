#ifndef LIBFILESYNC_TESTS_PROTOCOL_FTP_DELETE_REMOTE_FILE_HPP
#define LIBFILESYNC_TESTS_PROTOCOL_FTP_DELETE_REMOTE_FILE_HPP

#include <tests/protocol/ftp/ServerAccess.hpp>

namespace filesync::integration_test::protocol::ftp {

    class DeleteRemoteFile : public ServerAccess {

        public:
            DeleteRemoteFile(const std::string& testName,
                const std::string& server,
                const std::string& pathOnServer);

        private:
            void setup() override;
            void perform() override;
            void evaluate() override;
            void cleanUp() override;

    };

}

#endif