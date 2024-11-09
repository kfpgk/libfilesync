#ifndef LIBFILESYNC_FILE_SYNC_HPP
#define LIBFILESYNC_FILE_SYNC_HPP

#include <memory>
#include <filesystem>
#include <chrono>
#include <experimental/propagate_const>

namespace filesync {

    enum class ProtocolType {None, FTP};
    enum class ConflictResolveStrategy {None, LocalFirst, RemoteFirst};
    enum class SyncStrategy {None, UnBuffered, Buffered, OneWay};

    /**
     * @brief File Sync Client Interface Class
     * 
     * Patterns:
     *  - PIMPL
     */
    class FileSync {

        public:
            FileSync();
            ~FileSync();
            FileSync(const FileSync&) = delete;
            FileSync(FileSync&&);
            FileSync operator=(FileSync) = delete;

            void setProtocol(enum ProtocolType protocolType);
            void setServer(const std::string& address);
            void setRemoteRoot(const std::string& address);
            void setConflictResolveStrategy(enum ConflictResolveStrategy conflictResolveStrategy);
            void setSyncStrategy(enum SyncStrategy syncStrategy);
            void setSyncContent(const std::filesystem::path& path);
            void setSyncInvertal(std::chrono::milliseconds seconds);
            void startSyncing();
            void stopSyncing();

        private:
            class Impl;
            std::experimental::propagate_const<
                std::unique_ptr<Impl>> pImpl;

    };

}

#endif