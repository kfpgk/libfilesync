#ifndef LIBFILESYNC_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_PROTOCOL_CLIENT_HPP

#include <string>
#include <filesystem>

namespace filesync::protocol {

    /**
     * @brief: Protocol interface class
     * 
     * Non-virtual interface
     */
    class Client {

        public:
            Client() = default;
            explicit Client(const std::string& remoteRoot);
            virtual ~Client() = default;
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
            /**
             * @brief Deletes an entry on the server. This may be a file
             * or directory, depending on protocol support.
             * 
             * Throws if the entry does not exist.
             */
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

            void checkForEmptyPath(const std::filesystem::path& path);           

    };

}

#endif