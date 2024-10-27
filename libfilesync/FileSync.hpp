#ifndef LIBFILESYNC_FILE_SYNC_HPP
#define LIBFILESYNC_FILE_SYNC_HPP

#include <libfilesync/core/FileSyncer.hpp>
#include <libfilesync/data/Entry.hpp>
#include <libfilesync/protocol/ProtocolClient.hpp>

#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>
#include <thread>

using namespace std::chrono_literals;

namespace filesync {

    enum class ProtocolType {None, FTP};
    enum class ConflictResolveStrategy {None, LocalFirst, RemoteFirst};

    /**
     * Optional facade class for the file sync library.
     * 
     * Patterns:
     *  - Facade
     */
    class FileSync {

        public:
            FileSync() = default;
            ~FileSync();
            FileSync(const FileSync&) = delete;
            FileSync(FileSync&&) = delete;
            FileSync operator=(FileSync) = delete;

            void setProtocol(enum ProtocolType protocolType);
            void setServer(const std::string& address);
            void setRemoteRoot(const std::string& address);
            void setConflictResolveStrategy(enum ConflictResolveStrategy conflictResolveStrategy);
            void setSyncContent(const std::filesystem::path& path);
            void setSyncInvertal(std::chrono::milliseconds seconds);
            void startSyncing();
            void stopSyncing();

        private:
            std::string serverAddress = "";
            std::string remoteRoot = "";
            enum ProtocolType protocolType = ProtocolType::None;
            enum ConflictResolveStrategy conflictResolveStrategy = ConflictResolveStrategy::None;
            std::chrono::milliseconds interval = 5s;
            std::shared_ptr<ProtocolClient> protocolClient = nullptr;
            std::unique_ptr<Entry> entry = nullptr;
            std::unique_ptr<core::FileSyncer> fileSyncer = nullptr;

            std::thread syncThread;
            std::atomic<bool> syncing = false;
            std::atomic<bool> stopSyncThread = false;

            void createProtocol();
            void createFileSyncer();

            void validateProtocol() const;
            void validateEntry() const;

            /**
             * @brief Endless loop syncing for use 
             * in separate thread.
             */
            void endlessSync(const std::atomic<bool>& stop);

    };

}

#endif