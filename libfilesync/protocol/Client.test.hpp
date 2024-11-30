#ifndef LIBFILESYNC_PROTOCOL_CLIENT_TEST_HPP
#define LIBFILESYNC_PROTOCOL_CLIENT_TEST_HPP

#include <libfilesync/protocol/Client.hpp>

namespace filesync::protocol::unit_test {

    class ConcreteClient : public Client {

        private:
            virtual void doDownload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) {};          
            virtual void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) {};
            [[nodiscard]] virtual bool doExistsOnServer(
                const std::filesystem::path& remote) { return true; };
            virtual void doDeleteOnServer(
                const std::filesystem::path& remote) {};

    };

    class ClientTest {

        public:
            void delete_remote_file_with_empty_path();
            void download_file_with_empty_local_path();
            void download_file_with_empty_remote_path();
            void upload_file_with_empty_local_path();
            void upload_file_with_empty_remote_path();

    };

}

#endif