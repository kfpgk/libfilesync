#ifndef LIBFILESYNC_FILE_SYNC_HPP
#define LIBFILESYNC_FILE_SYNC_HPP

#include <memory>
#include <filesystem>
#include <chrono>
#include <experimental/propagate_const>

namespace filesync {

    enum class ProtocolType {None, FTP};
    enum class ConflictResolveStrategy {None, LocalFirst, RemoteFirst, Interactive};
    enum class SyncStrategy {None, UnBuffered, Buffered, OneWay};
    enum class BufferType {Default, MemoryBased, FileBased};

    class FileSyncLocks;

    /**
     * @brief File Sync Client Interface Class
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
            void setBufferType(enum BufferType bufferType);
            /**
             * @brief Start syncing blocking
             */
            void startSyncing();
            /**
             * @brief Start syncing non-blocking
             * 
             * This will start a new sync thread and returns.
             * The syncing thread may be using resources to which
             * access must be managed, so a set of mutexes can 
             * be provided via the FileSyncLocks object.
             */
            void startSyncing(std::shared_ptr<FileSyncLocks> locks);
            void stopSyncing();

        private:
            class Impl;
            std::experimental::propagate_const<
                std::unique_ptr<Impl>> pImpl;

    };

}

#endif