#ifndef LIBFILESYNC_PROTOCOL_CLIENT_TEST_HPP
#define LIBFILESYNC_PROTOCOL_CLIENT_TEST_HPP

#include <libfilesync/protocol/Client.hpp>
#include <libfilesync/protocol/memory/Handle.hpp>

#include <memory>
#include <span>

namespace filesync::protocol::unit_test {

    class ConcreteClient : public Client {

        private:
            void doDownload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override {};
            void doDownload(
                std::unique_ptr<memory::Handle<char>>& local,
                const std::filesystem::path& remote) override {};          
            void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) override {};
            void doUploadFromMemory(
                std::span<char> local,
                const std::filesystem::path& remote) override {};
            [[nodiscard]] bool doExistsOnServer(
                const std::filesystem::path& remote) override { return true; };
            void doDeleteOnServer(
                const std::filesystem::path& remote) override {};

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