#ifndef LIBFILESYNC_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_PROTOCOL_CLIENT_HPP

#include <string>
#include <filesystem>

namespace filesync {

    /**
     * @brief: Protocol interface class
     * 
     * Non-virtual interface
     */
    class ProtocolClient {

        public:
            ProtocolClient() = default;
            explicit ProtocolClient(const std::string& remoteRoot);
            virtual ~ProtocolClient() = default;
            void download(const std::filesystem::path& local);
            void download(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            void upload(const std::filesystem::path& local);
            void upload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            void setRemoteRootPath(
                const std::filesystem::path& remoteRoot);
            [[nodiscard]] bool existsOnServer(const std::filesystem::path& remote);
            void deleteOnServer(const std::filesystem::path& remote);

        protected:
            [[nodiscard]] std::filesystem::path getCompleteRemoteFilePath(
                const std::filesystem::path& remoteFile) const;

        private: 
            std::filesystem::path remoteRoot = "";

            virtual void doDownload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) = 0;          
            virtual void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) = 0;
            [[nodiscard]] virtual bool doExistsOnServer(
                const std::filesystem::path& remote) = 0;
            virtual void doDeleteOnServer(
                const std::filesystem::path& remote) = 0;           

    };

}

#endif