#ifndef LIBFILESYNC_PROTOCOL_CLIENT_HPP
#define LIBFILESYNC_PROTOCOL_CLIENT_HPP

#include <libfilesync/protocol/memory/Handle.hpp>
#include <libfilesync/protocol/HandleOrFilePath.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <variant>

namespace filesync::protocol {

    /**
     * @brief: Protocol interface class
     * 
     * Patterns:
     * 
     *  - Non-virtual interface
     */
    class Client {

        public:
            Client() = default;
            explicit Client(const std::string& remoteRoot);
            virtual ~Client() = default;
            /**
             * @brief Download from remote file (or directory)
             * to local file (or directory).
             */
            void download(const std::filesystem::path& local);
            /**
             * @brief Download from remote file (or directory)
             * to local file (or directory).
             */
            void download(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            /**
             * @brief Download from remote file into local
             * memory buffer.
             * 
             * The unique_ptr `local` will be reassigned to a newly
             * created handle to the downloaded data.
             * Data that was being hold by the handle before the 
             * function call, will be deleted.
             */
            void download(
                std::unique_ptr<memory::Handle<char>>& local,
                const std::filesystem::path& remote);
            /**
             * @brief Download from remote file to local
             * file or into local memory buffer depending
             * on `local` parameter type.
             */
            void download(
                HandleOrFilePath local,
                const std::filesystem::path& remote);
            /**
             * @brief Upload from local file (or directory)
             * to remote file (or directory).
             */
            void upload(const std::filesystem::path& local);
            /**
             * @brief Upload from local file (or directory)
             * to remote file (or directory).
             */
            void upload(
                const char* local,
                const std::filesystem::path& remote);
            /**
             * @brief Upload from local file (or directory)
             * to remote file (or directory).
             */
            void upload(
                const std::string& local,
                const std::filesystem::path& remote);
            /**
             * @brief Upload from local file (or directory)
             * to remote file (or directory).
             */
            void upload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote);
            /**
             * @brief Upload from memory buffer (handle)
             * to remote file.
             */
            void upload(
                std::unique_ptr<memory::Handle<char>>& local,
                const std::filesystem::path& remote);
            /**
             * @brief Upload from memory (char array)
             * to remote file.
             */
            void upload(
                std::span<char> local,
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
            virtual void doDownload(
                std::unique_ptr<memory::Handle<char>>& local,
                const std::filesystem::path& remote) = 0;        
            virtual void doUpload(
                const std::filesystem::path& local,
                const std::filesystem::path& remote) = 0;
            virtual void doUploadFromMemory(
                std::span<char> local,
                const std::filesystem::path& remote) = 0;
            [[nodiscard]] virtual bool doExistsOnServer(
                const std::filesystem::path& remote) = 0;
            virtual void doDeleteOnServer(
                const std::filesystem::path& remote) = 0;

            void checkForEmptyPath(const std::filesystem::path& path);           

    };

}

#endif